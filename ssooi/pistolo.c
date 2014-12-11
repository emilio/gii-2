/**
 * Pistolo.c
 *
 * http://avellano.usal.es/~ssooi/pract115.htm
 *
 * @author Emilio Cobos Álvarez <emiliocobos@usal.es>
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* strlen */
#include <time.h> /* time */
#include <unistd.h> /* getpid, write, close, unlink, fork */
#include <signal.h> /* sigset_t, sigsuspend, sigprocmask, sigdelset, sigaddset */
#include <fcntl.h> /* open */
#include <sys/mman.h> /* mmap, munmap */

#if __GNUC__
#	if __x86_64__ || __ppc64__
#		define PID_T_FORMAT "%d"
#	else
#		define PID_T_FORMAT "%ld"
#	endif
#else
#	warning "Could not detect arch. Assuming 32x"
#	define PID_T_FORMAT "%ld"
#endif



#define DEBUG
#define DEFAULT_PROC_COUNT 7
#define DATA_PATH "__pistolo_data.tmp"

/** Maximize the use of system calls */
#define __MAX_SYSTEM_CALLS

/** We use dynamic memory so we don't need to check for size limitations. I provide this option though */
#define LIMIT_MIN 3
#define LIMIT_MAX 128
#define CHECK_LIMITS 0

#ifdef __MAX_SYSTEM_CALLS
/**
 * Easy printf with system calls
 */
#define BUFF_MAX 256
char BUFF[BUFF_MAX];
#define PRINTF(str, ...) do { \
	snprintf(BUFF, BUFF_MAX, str, ## __VA_ARGS__); \
	write(1, BUFF, strlen(BUFF)); \
} while (0)
#else
#define PRINTF printf
#endif /* __MAX_SYSTEM_CALLS */

/** The  action type passed to manage_data */
typedef enum DataActionType {
	DATA_CREATE,
	DATA_REFRESH,
	DATA_GET,
	DATA_RELEASE
} DataActionType;

/**
 * The different pid statuses, see below
 *
 * This is a binary mask meaning that:
 *  - A process has shot if his pid_status & PID_STATUS_SHOT gives a non-zero value
 *  - A process is dead if his pid_status & PID_STATUS_DEAD gives a non-zero value
 */
typedef enum PidStatusType {
	PID_STATUS_READY = 0x01,
	PID_STATUS_SHOT = 0x02,
	PID_STATUS_DEAD_THIS_ROUND = 0x04,
	PID_STATUS_DEAD = 0x08
} PidStatusType;

/** Process struct */
typedef struct Process {
	pid_t id; /** PID */
	PidStatusType status; /** Current status, if pid is alive it gets updated to PID_STATUS_READY */
	struct Process *target; /** Current target */
} Process;

/**
 * The main struct
 * See the comment on each member for a quick description
 *
 * IMPORTANT (non-written) rules:
 *  - The parent pid can modify anything in the struct
 *  - A process can't modify anything in the struct except
 *    its corresponding `Process` struct
 */
typedef struct GameData {
	/* const */ size_t process_count; /** Number of processes playing. This is inmutable */
	/* const */ pid_t parent_id; /** The parent process id */
	size_t rounds; /** The round count */
	size_t alive_count; /** Current round alive pids count */
	Process *children; /** The children processes */
} GameData;

/** Get the main game data struct, create if non-zero value is passed as a second argument */
GameData * manage_data(DataActionType, size_t);

#define DATA_SIZE_FOR(count) (sizeof(GameData) + sizeof(Process) * count)

/** Wrapper macros for GameData actions */
#define get_data() manage_data(DATA_GET, 0)
#define refresh_data() manage_data(DATA_REFRESH, 0)
#define release_data() manage_data(DATA_RELEASE, 0)
#define create_data(process_count) manage_data(DATA_CREATE, process_count)

/** Loop helper. item is a pointer */
#define EACH(arr, length, item, ...) do { \
	size_t __i; \
	for ( __i = 0; __i < length; ++__i ) { \
		item = &arr[__i]; \
		__VA_ARGS__ \
	} \
} while (0)

#define EACH_WITH_INDEX(arr, length, item, index, ...) do { \
	for ( index = 0; index < length; ++index ) { \
		item = &arr[index]; \
		__VA_ARGS__ \
	} \
} while (0)

/** Another helper. Instead of signal() (implementation dependent), we use sigaction */
#define BIND_TO(signal, handler) do { \
	struct sigaction action; \
	/** Block everything during execution */ \
	sigfillset(&action.sa_mask); \
	action.sa_flags = 0; \
	action.sa_handler = handler; \
	sigaction(signal, &action, NULL); \
} while (0)

/** Empty signal handler for resuming (empty) */
void resume(int s) {};

/** Dump data */
void __dump();

/** Check if all processes are ready */
int all_ready();

/** Check if current round is over */
int round_over();

/** Show pids */
void show_pids();

/** Kill all pending processes */
void kill_all();

/** Catch SIGUSR1 signal (shoot to a brother) */
void child_sigusr_catch(int);

/** Catch SIGTERM signal (die) */
void child_sigterm_catch(int);

/** Catch sigterm signal, kill everything */
void parent_sigterm_catch(int);

/** Bind signals to parent proc only*/
void bind_parent_signals();

/** Bind signals to children proc */
void bind_children_signals();

/** Get a random brother Process. Receives the index by reference */
Process * child_rand_proc();

/** Get current proc */
Process * current_proc();

/** Child process logic, mostly wait for signals */
int child_proc();

/** Parent process logic */
int parent_proc();

/** Dump data */
void __dump() {
	GameData *data = get_data();
	size_t i;
	Process *p;

	PRINTF("\nDump:\n");
	EACH_WITH_INDEX(data->children, data->process_count, p, i,
		if ( data->rounds > 1 )
			PRINTF("{ "PID_T_FORMAT", %d, "PID_T_FORMAT" }\t", p->id, p->status, p->target->id);
		else
			PRINTF("{ "PID_T_FORMAT", %d }\t", p->id, p->status);
	);
}

/** Get current child process, cached statically */
Process * current_proc() {
	static Process *me = NULL;

	if ( me == NULL ) {
		GameData *data = get_data();
		Process *p;
		pid_t current_pid = getpid();
		EACH(data->children, data->process_count, p,
			if ( p->id == current_pid ) {
				me = p;
				break;
			}
		);
	}

	return me;
}

/** Manage game data */
GameData * manage_data(DataActionType action, size_t count) {
	static GameData *data = NULL;
	static int fd = -1;
	static size_t size = 0;

	/** Create data */
	if ( action == DATA_CREATE && count != 0 && data == NULL ) {
		/** Temporary buffer to write to file */
		void *buff;

		size = DATA_SIZE_FOR(count);

		/** Open our file */
		/* fd = shm_open(DATA_PATH, O_RDWR | O_CREAT | O_TRUNC, 0666); */
		fd = open(DATA_PATH, O_RDWR | O_CREAT | O_TRUNC, 0666);

		if ( fd == -1 ) {
			perror("File could not be created");
			exit(1);
		}

		/** Write our buffer to our file, and discard it, the content doesn't matter */
		buff = malloc(size);
		write(fd, buff, size);
		free(buff);

		/** Get our shared pointer and initialize it */
		data = (GameData *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		data->process_count = count;
		data->alive_count = count;
		data->parent_id = 0;
		data->rounds = 0;
		/** Adjust the pointer to the end of the struct */
		data->children = (Process *) (data + 1);
	}

	/** Release data */
	if ( action == DATA_RELEASE ) {
		munmap( (void *) data, size);
		close(fd);
		unlink(DATA_PATH);
		data = NULL;
	}

	return data;
}

/** Show list of procs */
void show_pids() {
	GameData *data = get_data();
	Process *p;
	EACH(data->children, data->process_count, p,
		if ( ~p->status & PID_STATUS_DEAD )
			PRINTF(PID_T_FORMAT"\t", p->id);
	);
	PRINTF("\n");
}

/** Kill all pending processes */
void kill_all() {
	GameData *data = get_data();
	Process *p;

	EACH(data->children, data->process_count, p,
		kill(p->id, SIGKILL);
	);
}


/** Get a pid and shoot */
void child_sigusr_catch( int sig ) {
	GameData *data = get_data();
	Process *me = current_proc(),
		*target;

	/** Get our target */
	target = child_rand_proc();

	PRINTF(PID_T_FORMAT"->"PID_T_FORMAT"\n", me->id, target->id);
	kill(target->id, SIGTERM);

	me->target = target;
	me->status |= PID_STATUS_SHOT;

	/** Tell the parent we're done */
	kill(data->parent_id, SIGUSR2);
}

/** Receive shoot */
void child_sigterm_catch( int sig ) {
	GameData *data = get_data();
	Process *me = current_proc();

	/** Fake the shot if we haven't shooted yet */
	if ( ~me->status & PID_STATUS_SHOT )
		child_sigusr_catch(SIGUSR1);

	me->status |= PID_STATUS_DEAD_THIS_ROUND;

	/** Tell parent we're done and exit */
	kill(data->parent_id, SIGUSR2);

	exit(0);
}

/** Our parent was interrupted, dump data for debugging and release all */
void parent_sigterm_catch(int sig) {
	__dump();
	kill_all();
	release_data();
	exit(1);
}

/** Bind signals to parent proc so children */
void bind_children_signals() {
	sigset_t set;

	/** We only want to receive SIGUSR1 and SIGTERM while we're waiting for them */
	sigemptyset(&set);
	sigaddset(&set, SIGTERM);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);

	/** Shoot */
	BIND_TO(SIGUSR1, child_sigusr_catch);

	/** Receive shoot */
	BIND_TO(SIGTERM, child_sigterm_catch);
}

void bind_parent_signals() {
	sigset_t set;

	/** We only want to receive SIGUSR2 while we're waiting for it */
	sigemptyset(&set);
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &set, NULL);

	/** Ending signals */
	BIND_TO(SIGTERM, parent_sigterm_catch);
	BIND_TO(SIGINT, parent_sigterm_catch);

	/** Children signals: ignore, just continue execution. Alarm is just used as a fallback. */
	BIND_TO(SIGUSR2, resume);
}

/** Get a random Process. TODO: improve it. Maybe create a pid_t array with round alive pids? */
Process * child_rand_proc() {
	pid_t current_pid = getpid();
	GameData *data = get_data();
	Process *p;
	size_t random;

	/** All alives except us */
	random = rand() % (data->alive_count - 1);

	EACH(data->children, data->process_count, p,
		/** If not dead in previous rounds and not self */
		if ( ~p->status & PID_STATUS_DEAD && p->id != current_pid ) {
			if ( random == 0 )
				return p;

			random--;
		}
	);

	return NULL;
}

/** Child process subroutine */
int child_proc() {
	sigset_t set;
	GameData *data = get_data();

	bind_children_signals();

	/** seed the PRNG with getpid() to get different values for different procs */
	srand(time(NULL) + getpid());

	/** Only allow to catch SIGUSR1 and SIGTERM */
	sigfillset(&set);
	sigdelset(&set, SIGUSR1);
	sigdelset(&set, SIGTERM);

	/** We update our status, and send a signal to the parent */
	current_proc()->status = PID_STATUS_READY;
	kill(data->parent_id, SIGUSR2);

	/** Wait until the end */
	while ( 1 )
		sigsuspend(&set);

	return 0;
}

/** Check if all children are ready */
int all_ready() {
	GameData *data = get_data();
	Process *p;

	EACH(data->children, data->process_count, p,
		if ( p->status != PID_STATUS_READY )
			return 0;
	);

	return 1;
}

/**
 * Check if current round is over
 * Current round is over if none of pid_status is PID_STATUS_SHOT
 */
int round_over() {
	GameData *data = get_data();
	Process *p;

	EACH(data->children, data->process_count, p,
		/* If someone hasn't shooted yet and isn't dead */
		if ( p->status == PID_STATUS_READY )
			return 0;
		/* Everyone must shoot */
		else if ( ~p->status & PID_STATUS_SHOT )
			return 0;
		/* If someone has shooted but it's target isn't dead */
		else if ( p->status & PID_STATUS_SHOT && ~p->target->status & PID_STATUS_DEAD_THIS_ROUND )
			return 0;
	);

	return 1;
}

/** Parent process subroutine */
int parent_proc() {
	size_t already_dead,
		total_dead = 0;
	GameData *data = get_data();
	Process *p;
	size_t count = data->process_count;
	sigset_t set;

	sigfillset(&set);

	/** Parent waits for sigusr2 */
	sigdelset(&set, SIGUSR2);

	/** For debugging and stopping the program */
	sigdelset(&set, SIGTERM);
	sigdelset(&set, SIGINT);

	PRINTF("Waiting for ready state... ");

	/** Wait until all processes are ready */
	while ( ! all_ready() )
		sigsuspend(&set);

	PRINTF("done\n");

	/** Main loop */
	while ( 1 ) {
		already_dead = total_dead;
		total_dead = 0;

		EACH(data->children, data->process_count, p,
			/** Check for deads in the last round, and set ready if not dead */
			if ( p->status & PID_STATUS_DEAD_THIS_ROUND )
				p->status |= PID_STATUS_DEAD;

			if ( p->status & PID_STATUS_DEAD )
				++total_dead;
			else
				p->status = PID_STATUS_READY;
		);

		/** If game is over (all dead or all minus one) */
		if ( total_dead == count || total_dead == count - 1 )
			break;

		++data->rounds;
		data->alive_count = count - total_dead;

		PRINTF("\n----------------------\n");
		PRINTF("Round #%zu\n", data->rounds);
		PRINTF("Total: %zu\n", count);
		PRINTF("Dead: %zu\n", total_dead);
		PRINTF("Last round: %zu\n", total_dead - already_dead);
		PRINTF("Alive (%zu):\n", count - total_dead);
		show_pids();
		PRINTF("----------------------\n");

		/** Let them shoot */
		EACH(data->children, data->process_count, p,
			if ( ~p->status & PID_STATUS_DEAD )
				kill(p->id, SIGUSR1);
		);

		/** Wait for children until round is over */
		while ( ! round_over() )
			sigsuspend(&set);
	}

	/** Game is over! Print results, kill all pending procs and release data */
	PRINTF("Game is over!\n");

	if ( total_dead == count )
		PRINTF("No shooter is alive\n");
	else
		PRINTF("%zu shooters alive\n", count - total_dead); /* Only should be one */

	show_pids();

	kill_all();
	release_data();

	return 0;
}

/** Main process logic */
int main(int argc, char **argv) {
	size_t count = DEFAULT_PROC_COUNT;
	pid_t current_pid;
	GameData *data;

	if ( argc > 1 )
		count = strtoul(argv[1], NULL, 10);

#if CHECK_LIMITS
	if ( count < LIMIT_MIN || count > LIMIT_MAX ) {
		PRINTF("Number of processes must be between %d and %d\n", LIMIT_MIN, LIMIT_MAX);
		return 1;
	}
#endif

	PRINTF("Starting %s with %zu processes\n", argv[0], count);

	/** Create shared data structure */
	data = create_data(count);

	data->parent_id = getpid();

	bind_parent_signals();

	/** Process creation */
	while ( count-- ) {
		/** All processes start as dead */
		data->children[count].status = PID_STATUS_DEAD;
		current_pid = fork();
		switch ( current_pid ) {
			case -1:
				perror("fork");
				__dump();
				kill_all();
				release_data();
				exit(1);
			case 0:
				/** We must ensure current_index is ok in child_proc */
				data->children[count].id = getpid();
				return child_proc();
		}
	}

	return parent_proc();
}
