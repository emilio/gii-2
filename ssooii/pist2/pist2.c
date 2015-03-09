/**
 * Pistolo.c
 *
 * http://avellano.usal.es/~ssooi/pract115.htm
 *
 * @author Emilio Cobos Álvarez <emiliocobos@usal.es>
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h> /* strlen, strerror */
#include <time.h> /* time */
#include <unistd.h> /* getpid, write, close, unlink, fork */
#include <signal.h> /* sigset_t, sigsuspend, sigprocmask, sigdelset, sigaddset */
#include <fcntl.h> /* open */
#include <sys/mman.h> /* mmap, munmap */
#include <sys/types.h>
#include <sys/wait.h> /** waitpid, WNOHANG */
#include <sys/sem.h>
#include "pist2.h"

#define SELF "pistolos"

#ifdef __GNUC__
#	if __x86_64__ || __ppc64__
#		define PID_T_FORMAT "%d"
#	else
#		define PID_T_FORMAT "%ld"
#	endif
#else
#	warning "Could not detect arch. Assuming 32x"
#	define PID_T_FORMAT "%ld"
#endif

/** Debug compilation macro */
/* #define __DEBUG */

#ifdef __GNUC__
#    define ATOMIC_DECREMENT_AND_FETCH(var) (__sync_sub_and_fetch(var, 1))
#    define ATOMIC_INCREMENT_AND_FETCH(var) (__sync_add_and_fetch(var, 1))
#elif _WIN32
#    define ATOMIC_DECREMENT_AND_FETCH(var) (InterlockedDecrement(var))
#    define ATOMIC_INCREMENT_AND_FETCH(var) (InterlockedIncrement(var))
#else
#    error "Atomic decrement is required"
#endif

/** Default process count and output */
#define DEFAULT_PROC_COUNT 7
#define DEFAULT_SPEED 1

/** One for the library, two for me */
#define TOTAL_SEMAPHORE_COUNT 5

/** Easy printf with system calls */
#define BUFF_MAX 256
char BUFF[BUFF_MAX];

/** Error */
#define ERROR(str, ...) do { \
	char buff[512]; \
	snprintf(buff, 512, str, ##__VA_ARGS__); \
	pon_error(buff); \
} while ( 0 )
/* #define ERROR(str, ...) do { \
	fprintf(stderr, "(error) "str, ## __VA_ARGS__); \
	fprintf(stderr, "Run with --help for a list of options.\n"); \
} while (0) */

#define FATAL_ERROR(str, ...) do { \
	ERROR(str, ##__VA_ARGS__); \
	exit(1); \
} while( 0 )

/** Debug */
#ifdef __DEBUG
char VERBOSE_MODE = 0;
#define DEBUG(str, ...) do { \
	if ( VERBOSE_MODE ) \
		fprintf(stderr, "(debug) "str, ## __VA_ARGS__); \
} while (0)
#else
#define DEBUG(...)
#endif

/** We use dynamic memory so we don't need to check for size limitations. I provide this option though */
#define LIMIT_MIN 3
#define LIMIT_MAX 128
#define CHECK_LIMITS 1

/** To be a bit clearer */
typedef int semaphore_t;
typedef unsigned char bool;

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
	volatile PidStatusType status; /** Current status, if pid is alive it gets updated to PID_STATUS_READY */
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
	semaphore_t semaphores;
	char library_data[256];
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

/** Main semaphore utilities */
int semaphore_set_value(semaphore_t sem, unsigned short index, int val) {
	int ret = semctl(sem, index, SETVAL, val);

	if ( ret == -1 )
		FATAL_ERROR("semaphore_set_value: %s", strerror(errno));

	return ret;
}

int semaphore_get_value(semaphore_t sem, unsigned short index) {
	int ret = semctl(sem, index, GETVAL);

	if ( ret == -1 )
		FATAL_ERROR("semaphore_set_value: %s", strerror(errno));

	return ret;
}

#define MAIN_SEMAPHORE_INDEX 1
int semaphore_wait_zero(semaphore_t sem, unsigned short index) {
	static struct sembuf buff = { 0, 0, 0 };
	buff.sem_num = index;
	int ret = semop(sem, &buff, 1);
	if ( ret == -1 )
		FATAL_ERROR("semaphore_wait_zero: %s", strerror(errno));
	return ret;
}

/** Equivalent to `wait` */
int semaphore_lock(semaphore_t sem, unsigned short index) {
	static struct sembuf buff = { 0, -1, 0 };
	buff.sem_num = index;
	int ret = semop(sem, &buff, 1);
	if ( ret == -1 )
		FATAL_ERROR("semaphore_lock: %s", strerror(errno));
	return ret;
}

/** Equivalent to `signal` */
int semaphore_unlock(semaphore_t sem, unsigned short index) {
	static struct sembuf buff = { 0, 1, 0 };
	buff.sem_num = index;
	int ret = semop(sem, &buff, 1);
	if ( ret == -1 )
		FATAL_ERROR("semaphore_unlock: %s", strerror(errno));
	return ret;
}

/** Empty signal handler for resuming (empty) */
void resume(int s) {};

/** Dump data */
void __dump();

/** Display program help */
void program_help();

/** Kill all pending processes */
void kill_all();

/** Catch sigterm signal, kill everything */
void parent_sigterm_catch(int);

/** Bind signals to parent proc only*/
void bind_parent_signals();

/** Bind signals to children proc */
void bind_children_signals();

/** Get current proc */
Process * current_proc();

/** Triggers when child is ready */
void child_ready();

/** Child process logic, mostly wait for signals */
int child_proc(char);

/** Parent process logic */
int parent_proc();

/** Dump data */
void __dump() {
	GameData *data = get_data();
	size_t i;
	Process *p;

	printf("\nDump:\n");
	printf("Semaphores:\n");
	for ( i = 0; i < TOTAL_SEMAPHORE_COUNT; ++i )
		printf("\t%d", semaphore_get_value(data->semaphores, i));
	printf("\n");

	EACH_WITH_INDEX(data->children, data->process_count, p, i,
		printf("{ "PID_T_FORMAT", %d}\t", p->id, p->status);
	);
	printf("\n");
}

/** Program help */
void program_help() {
	printf("Usage: "SELF" [count=%d] [-d|--debug] [-o|--output <output_file>]\n", DEFAULT_PROC_COUNT);
	printf("Options:\n");
	printf("\t-h\t--help\t\tShow this message\n");
#ifdef __DEBUG
	printf("\t-v\t--verbose\t\tEnable debugging\n");
#endif
	printf("\t-o\t--output\t<filename>\tChange output from console to a file.\n");
	printf("\t\tNote: this option is not reliable actually, prefer the use of `>>`\n");
	exit(1);
}

/** Get current child process, cached statically */
Process *current_proc() {
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

bool is_current_proc_coordinator() {
	Process* p;
	GameData* data = get_data();
	pid_t current_pid = getpid();

	EACH(data->children, data->process_count, p,
		/** If there's a process which is not dead */
		if ( ~p->status & PID_STATUS_DEAD ) {
			if ( p->id == current_pid )
				return 1;
			else
				return 0;
		}
	);

	return 0;
}

/** Manage game data */
GameData * manage_data(DataActionType action, size_t count) {
	static GameData *data = NULL;
	static size_t size = 0;

	/** Create data */
	if ( action == DATA_CREATE && count != 0 && data == NULL ) {
		size = DATA_SIZE_FOR(count);

		/** Get our shared pointer and initialize it */
		data = (GameData *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		data->process_count = count;
		data->alive_count = count;
		data->parent_id = 0;
		data->rounds = 0;
		data->semaphores = 0;
		/** Adjust the pointer to the end of the struct */
		data->children = (Process *) (data + 1);
	}

	/** Release data */
	if ( action == DATA_RELEASE ) {
		/** TODO: Is count ignored in semctl with RMID? */
		if ( data->semaphores > 0 )
			semctl(data->semaphores, 0, IPC_RMID);

		munmap( (void *) data, size);
		data = NULL;
	}

	return data;
}

/** Kill all pending processes */
void kill_all() {
	GameData *data = get_data();
	Process *p;

	EACH(data->children, data->process_count, p,
		kill(p->id, SIGKILL);
	);
}

/** Our parent was interrupted, dump data for debugging and release all */
void parent_sigterm_catch(int sig) {
	__dump();
	kill_all();
	release_data();
	fflush(stderr);
	fclose(stderr);
	exit(1);
}

/** Children have all signals blocked. */
void bind_children_signals() {
	sigset_t set;

	sigfillset(&set);
	sigprocmask(SIG_BLOCK, &set, NULL);
}

/** Parent listens for sigint and sigterm */
void bind_parent_signals() {
	sigset_t set;

	sigfillset(&set);
	sigdelset(&set, SIGINT);
	sigdelset(&set, SIGTERM);
	sigprocmask(SIG_BLOCK, &set, NULL);

	BIND_TO(SIGTERM, parent_sigterm_catch);
	BIND_TO(SIGINT, parent_sigterm_catch);
}

/** Child process subroutine */
int child_proc(char lib_id) {
	volatile GameData* data = get_data();
	volatile Process* me = current_proc();
	char target;
	int i;
	bool im_coordinator;

	bind_children_signals();

	/** We update our status, and send a signal to the parent */
	me->status = PID_STATUS_READY;

	PIST_nuevoPistolero(lib_id);
	/** Ensure we're ready, and wait for everybody else */
	semaphore_lock(data->semaphores, 1);
	semaphore_wait_zero(data->semaphores, 1);
	// ERROR("All ready");

	/** Parent takes care of unlocking... */
	while ( 1 ) {
		/** Check if I'm the coordinator */
		im_coordinator = is_current_proc_coordinator();

		/** If I am, give pass to everyone two times: one for start and one for end */
		if ( im_coordinator ) {
			// ERROR("Coordinator: %c", lib_id);
			if ( data->alive_count == 1 )
				exit(0);
			data->rounds++;
			/** Set all semaphores for everybody */
			semaphore_set_value(data->semaphores, 1, data->alive_count);
			semaphore_set_value(data->semaphores, 3, data->alive_count);
			semaphore_set_value(data->semaphores, 2, data->alive_count); // Now everyone locked there will pass
		}

		/** Wait for everybody to start... Here even the coordinator passes  */
		semaphore_lock(data->semaphores, 2);

		target = PIST_vIctima();

		if ( target == '@' )
			FATAL_ERROR("This shit blew up\n");

		/** Mark the target as shot, equivalent to send the "DEAD" message */
		// me->status |= PID_STATUS_SHOT;
		data->children[target - 'A'].status |= PID_STATUS_DEAD_THIS_ROUND;

		if ( PIST_disparar(target) == -1 )
			ERROR("PIST_disparar");

		/** LOCK */
		semaphore_lock(data->semaphores, 1);
		semaphore_wait_zero(data->semaphores, 1);

		/** Here we know that everyone has shooted: Prevent everybody to start the next round without dying */
		if ( im_coordinator )
			semaphore_set_value(data->semaphores, 2, 0);

		if ( me->status & PID_STATUS_DEAD_THIS_ROUND ) {
			me->status |= PID_STATUS_DEAD;
			ATOMIC_DECREMENT_AND_FETCH(&data->alive_count);
			PIST_morirme();
		} else {
			me->status = PID_STATUS_READY;
		}

		/** No lock required here */
		semaphore_lock(data->semaphores, 3);
		semaphore_wait_zero(data->semaphores, 3);

		/** Die if round over */
		if ( me->status & PID_STATUS_DEAD )
			exit(0);
	}

	return 0;
}

/** Main process logic */
int main(int argc, char **argv) {
	size_t i = 0,
		count = DEFAULT_PROC_COUNT,
		speed = DEFAULT_SPEED;
	pid_t current_pid;
	GameData *data;
	int ret;
	char lib_id = 'A';

	/** check arguments */
	for ( i = 1; i < argc; ++i ) {
		if ( strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0 ) {
			program_help();
#ifdef __DEBUG
		} else if ( strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0 ) {
			VERBOSE_MODE = 1;
#endif
		/** If first char is numeric we assume its the proc count */
		} else if ( argv[i][0] >= '0' && argv[i][0] <= '9' ) {
			count = strtoul(argv[i], NULL, 10);
		} else {
			FATAL_ERROR("Option not recognized: \"%s\".\n", argv[i]);
		}
	}

#if CHECK_LIMITS
	if ( count < LIMIT_MIN || count > LIMIT_MAX )
		FATAL_ERROR("Number of processes must be between %d and %d.\n", LIMIT_MIN, LIMIT_MAX);
#endif

	if ( count == 0 )
		FATAL_ERROR("At least one player is required.\n");

	DEBUG(SELF": Verbose mode enabled.\n");

	printf("Starting %s with %zu processes.\n", argv[0], count);

	/** Create shared data structure */
	data = create_data(count);

	/** Another for us */
	data->semaphores = semget(IPC_PRIVATE, TOTAL_SEMAPHORE_COUNT, IPC_CREAT | 0600);

	if ( data->semaphores == -1 )
		FATAL_ERROR("Semaphore creation failed: %s\n", strerror(errno));

	ret = PIST_inicio(count, speed, data->semaphores, data->library_data, 0);

	if ( ret == -1 )
		FATAL_ERROR("Library initialization failed\n");

	data->parent_id = getpid();

	bind_parent_signals();

	/** Set the first semaphore to `count`: we'll wait to 0 in the main proc to wait until they are all ready */
	semaphore_set_value(data->semaphores, 1, count);
	/** Set the second one to zero */
	semaphore_set_value(data->semaphores, 2, count);

	/** Process creation */
	for ( i = 0; i < count; ++i ) {
		/** All processes start as dead */
		data->children[i].status = PID_STATUS_DEAD;
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
				data->children[i].id = getpid();
				return child_proc(lib_id);
		}
		lib_id++;
	}

	/** Wait for all processes to die */
	while ( count-- )
		waitpid(0, NULL, 0);

	/** When game is over... */
	ret = PIST_fin();
	if ( ret == -1 )
		DEBUG("Library termination failed\n");

	kill_all();
	release_data();
	return 0;
}
