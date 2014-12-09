/**
 * Pistolo.c
 *
 * http://avellano.usal.es/~ssooi/pract115.htm
 *
 * @author Emilio Cobos Álvarez <emiliocobos@usal.es>
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#define DEFAULT_PROC_COUNT 7
#define DATA_PATH "/tmp/__pistolo_data"

/**
 * The description says that every alive process MUST shoot each round
 * From my point of view this behaviour is unwanted, so I allow disabling
 * this functionality
 */
#define __CONFORMING 1


/**
 * Easy printf with system calls
 */
#define BUFF_MAX 256
char BUFF[BUFF_MAX];
#define PRINTF(str, ...) do { \
	snprintf(BUFF, BUFF_MAX, str, ## __VA_ARGS__); \
	write(1, BUFF, strlen(BUFF)); \
} while (0)

/**
 * The GameData struct is the main data wrapper for our game
 *
 * With the function manage_data() and three helper macros we can manage it
 */

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
enum PidStatusType {
	PID_STATUS_READY = 0x01,
	PID_STATUS_SHOT = 0x02,
	PID_STATUS_DEAD_THIS_ROUND = 0x04,
	PID_STATUS_DEAD = 0x08
};

typedef volatile enum PidStatusType PidStatusType;

/**
 * The struct itself,
 * See the comment on each member for a quick description
 *
 * IMPORTANT (non-written) rules:
 *  - The parent pid can modify anything in the struct
 *  - A child pid can't modify anything in the struct except:
 *       + His index in the pids array
 *       + His index in the pid_status array
 *  - A dead pid takes the pid_t value of zero and an index status type with PID_STATUS_DEAD
 */
typedef struct GameData {
	/* const */ size_t process_count; /** Number of processes playing. This is inmutable */
	/* const */ pid_t parent_id; /** The parent process id */
	size_t rounds; /** The round count */
	size_t alive_count; /** Current round alive pids count */
	pid_t *pids; /** The pid array, dinamically allocated */
	PidStatusType *pid_status; /** The pid status array, dinamically allocated too.
									If pid is alive it gets updated to PID_STATUS_READY
									at the beginning of each round */
	sig_atomic_t *pid_target; /** An array of target INDEXES of each process
								    When a target index isn't dead the round isn't over yet */
} GameData;

/** Get the main game data struct, create if non-zero value is passed as a second argument */
GameData * manage_data(DataActionType, size_t);

#define DATA_SIZE_FOR(count) (sizeof(GameData) + sizeof(pid_t) * count + sizeof(PidStatusType) * count + sizeof(sig_atomic_t) * count)

/** Wrapper macros for GameData actions */
#define get_data() manage_data(DATA_GET, 0)
#define refresh_data() manage_data(DATA_REFRESH, 0)
#define release_data() manage_data(DATA_RELEASE, 0)
#define create_data(process_count) manage_data(DATA_CREATE, process_count)

/** Little helper macro: suspend with a set until some condition is false */
#define WAIT_WITH_SET_UNTIL(set, cond) do { \
	do { \
		alarm(1); \
		sigsuspend(&set); \
	} while ( ! (cond) ); \
	alarm(0); \
} while (0)

/** Signal handler for resuming (empty) */
void resume(int s) {};

/** Dump data */
void __dump();

/** Check if all processes are ready */
int all_ready();

/** Check if current round is over */
int round_over();

/** Display an integer list */
void print(int *, size_t);

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

/** Get a random brother PID. Receives the index by reference */
pid_t child_rand_pid(size_t *);

/** Get current child index */
size_t current_index();

/** Child process logic, mostly wait for signals */
int child_proc();

/** Parent process logic */
int parent_proc();

/** Dump data */
void __dump() {
	GameData *data = get_data();

	PRINTF("\nDump:\n");
	PRINTF(" * Pids: \n");
	print( (int *) data->pids, data->process_count);
	PRINTF(" * Status: \n");
	print( (int *) data->pid_status, data->process_count);
}

/** Get current child index */
size_t current_index() {
	static size_t index = (size_t) -1;

	if ( index == (size_t) -1 ) {
		GameData *data = get_data();
		pid_t current_pid = getpid();
		pid_t *pids = data->pids;
		size_t count = data->process_count,
			i = 0;

		for ( ; i < count; ++i ) {
			if ( pids[i] == current_pid ) {
				index = i;
				break;
			}
		}
	}

	return index;
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
		fd = open(DATA_PATH, O_RDWR | O_CREAT | O_TRUNC);

		if ( fd == -1 ) {
			perror("File could not be created");
			exit(1);
		}

		/** Write our buffer to our file, and discard it */
		buff = malloc(size);
		write(fd, buff, size);
		free(buff);

		/** Get our shared pointer and initialize it */
		data = (GameData *) mmap(NULL, size, PROT_WRITE, MAP_SHARED, fd, 0);
		data->process_count = count;
		data->alive_count = count;
		data->parent_id = 0;
		data->rounds = 0;
		data->pids =  (pid_t *) (data + 1);
		data->pid_status = (PidStatusType *) (data->pids + count);
		data->pid_target = (sig_atomic_t *) (data->pid_status + count);
	}

	/** Release data */
	if ( action == DATA_RELEASE ) {
		munmap(data, size);
		close(fd);
		unlink(DATA_PATH);
		data = NULL;
	}

	return data;
}

/** Print an integer list */
void print(int *list, size_t length) {
	size_t i = 0;

	for ( ; i < length; ++i )
		PRINTF("%d\t", list[i]);

	PRINTF("\n");
}

/** Show list of procs */
void show_pids() {
	GameData *data = get_data();
	pid_t *pids = data->pids;
	PidStatusType *pid_status = data->pid_status;
	size_t i = 0,
		count = data->process_count;

	for ( ; i < count; ++i )
		if ( ~pid_status[i] & PID_STATUS_DEAD )
			PRINTF("%d\t", pids[i]);

	PRINTF("\n");
}

/** Kill all pending processes */
void kill_all() {
	GameData *data = get_data();
	pid_t *pids = data->pids;
	size_t count = data->process_count;

	while ( count-- )
		if ( pids[count] )
			kill(pids[count], SIGKILL);
}

/** Get a pid and shoot */
void child_sigusr_catch( int sig ) {
	pid_t random_pid,
		current_pid = getpid();
	size_t random_pid_index;
	GameData *data = get_data();

	/** Get our target */
	random_pid = child_rand_pid(&random_pid_index);


	PRINTF("%d->%d\n", current_pid, random_pid);
	kill(random_pid, SIGTERM);

	data->pid_target[current_index()] = (sig_atomic_t) random_pid_index;
	data->pid_status[current_index()] |= PID_STATUS_SHOT;

	/** Tell the parent we're done */
	kill(data->parent_id, SIGUSR2);
}

/** Receive shoot */
void child_sigterm_catch( int sig ) {
	GameData *data = get_data();

#if __CONFORMING
	if ( ~data->pid_status[current_index()] & PID_STATUS_SHOT )
		child_sigusr_catch(SIGUSR1);
#endif

	data->pid_status[current_index()] |= PID_STATUS_DEAD_THIS_ROUND;

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

#if __CONFORMING
	/** Block sigterm and sigusr1 except when we have to wait for them */
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGTERM);
	sigprocmask(SIG_BLOCK, &set, NULL);
#endif

	/** Shoot */
	signal(SIGUSR1, child_sigusr_catch);

	/** Receive shoot */
	signal(SIGTERM, child_sigterm_catch);
}

void bind_parent_signals() {
	/** Ending signals */
	signal(SIGTERM, parent_sigterm_catch);
	signal(SIGINT, parent_sigterm_catch);

	/** Children signals: ignore, just continue execution. Alarm is just used as a fallback. */
	signal(SIGUSR2, resume);
	signal(SIGALRM, resume);
}

/** Get a random pid. TODO: improve it. Maybe create a pid_t array with round alive pids? */
pid_t child_rand_pid(size_t *index) {
	pid_t current_pid = getpid();
	GameData *data = get_data();
	size_t count = data->process_count,
		i = 0,
		random;
	pid_t *pids = data->pids;
	PidStatusType *pid_status = data->pid_status;

	/** All alives except us */
	random = rand() % (data->alive_count - 1);

	for ( i = 0;  i < count; ++i ) {
		/** If not dead in previous rounds and not self */
		if ( ~pid_status[i] & PID_STATUS_DEAD && pids[i] != current_pid ) {
			if ( random == 0 ) {
				if ( index != NULL )
					*index = i;
				return pids[i];
			}
			random--;
		}
	}

	return -1;
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
	data->pid_status[current_index()] = PID_STATUS_READY;
	kill(data->parent_id, SIGUSR2);

	/** We start listening */
	while ( 1 )
		sigsuspend(&set);

	return 0;
}

/** Check if all children are ready */
int all_ready() {
	GameData *data = get_data();
	PidStatusType *pid_status = data->pid_status;
	size_t count = data->process_count,
		i = 0;

	for ( ; i < count; ++i )
		if ( pid_status[i] != PID_STATUS_READY )
			return 0;

	return 1;
}

/**
 * Check if current round is over
 * Current round is over if none of pid_status is PID_STATUS_SHOT
 */
int round_over() {
	GameData *data = get_data();
	PidStatusType *pid_status = data->pid_status;
	sig_atomic_t *pid_target = data->pid_target;
	size_t count = data->process_count,
		i = 0;
	for ( ; i < count; ++i ) {
		/* If someone hasn't shooted yet and isn't dead */
		if ( pid_status[i] == PID_STATUS_READY )
			return 0;
		/* If someone has shooted but it's target isn't dead */
		else if ( pid_status[i] & PID_STATUS_SHOT && ~pid_status[pid_target[i]] & PID_STATUS_DEAD_THIS_ROUND )
			return 0;
	}

	return 1;
}

/** Parent process subroutine */
int parent_proc() {
	size_t
		i,
		already_dead,
		total_dead = 0;
	GameData *data = get_data();
	size_t count = data->process_count;
	pid_t *pids = data->pids;
	PidStatusType *pid_status = data->pid_status;
	sig_atomic_t *pid_target = data->pid_target;
	sigset_t set;

	sigfillset(&set);

	/** Parent waits for sigusr2 or alarm */
	sigdelset(&set, SIGUSR2);
	sigdelset(&set, SIGALRM);

	/** For debugging and stopping the program */
	sigdelset(&set, SIGTERM);
	sigdelset(&set, SIGINT);

	/** Wait until all processes are ready */
	WAIT_WITH_SET_UNTIL(set, all_ready());

	PRINTF("All ready to start\n");

	/** Main loop */
	while ( 1 ) {
		already_dead = total_dead;
		total_dead = 0;

		/** Check for deads in the last round, and set ready if not dead */
		for ( i = 0; i < count; ++i ) {
			if ( pid_status[i] & PID_STATUS_DEAD_THIS_ROUND )
				pid_status[i] |= PID_STATUS_DEAD;

			if ( pid_status[i] & PID_STATUS_DEAD ) {
				++total_dead;
			} else {
				pid_status[i] = PID_STATUS_READY;
				pid_target[i] = 0;
			}
		}

		/** If game is over (all dead or all minus one) */
		if ( total_dead == count || total_dead == count - 1 )
			break;

		++data->rounds;
		data->alive_count = count - total_dead;

		PRINTF("\n----------------------\n");
		PRINTF("Round #%zu\n", data->rounds);
		PRINTF("Total: %zu\n", ( size_t ) count);
		PRINTF("Dead: %zu\n", total_dead);
		PRINTF("Last round: %zu\n", total_dead - already_dead);
		PRINTF("Alive (%zu):\n", count - total_dead);
		show_pids();
		PRINTF("----------------------\n");

		/** Let them shoot */
		for ( i = 0; i < count; ++i )
			if ( ~pid_status[i] & PID_STATUS_DEAD )
				kill(pids[i], SIGUSR1);

		/** Wait until round is over */
		WAIT_WITH_SET_UNTIL(set, round_over());
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

	/** Create shared data structure */
	data = create_data(count);

	data->parent_id = getpid();

	bind_parent_signals();

	/** Process creation */
	while ( count-- ) {
		/** All processes start as dead */
		data->pid_status[count] = PID_STATUS_DEAD;
		current_pid = fork();
		if ( current_pid == 0 ) {
			/** We must ensure current_index is ok in child_proc */
			data->pids[count] = getpid();
			return child_proc();
		}

		data->pids[count] = current_pid;
	}


	return parent_proc();
}
