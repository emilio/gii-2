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
#include <assert.h>
#include <unistd.h> /* getpid, write, close, unlink, fork */
#include <signal.h> /* sigset_t, sigsuspend, sigprocmask, sigdelset, sigaddset */
#include <fcntl.h> /* open */
#include <sys/mman.h> /* mmap, munmap */
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h> /** waitpid, WNOHANG */
#include <sys/sem.h>

#ifdef STATEMENT_CONFORMANT
#	include <sys/msg.h>
#endif

#include "pist2.h"

union semun {
	int val;
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
};

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

#ifdef __GNUC__
#	if __GNUC__ < 4 /** Encina... -.- */
#       include <sys/atomic.h>
		/** Assume it's 2 bytes (short size)  */
#		define ATOMIC_DECREMENT(var) (atomic_dec_16_nv(var))
#	else
#		define ATOMIC_DECREMENT(var) (__sync_sub_and_fetch(var, 1))
#	endif
#elif _WIN32
#   define ATOMIC_DECREMENT(var) (InterlockedDecrement(var))
#else
#    error "Atomic decrement is required"
#endif

/** Default process count and output */
#define DEFAULT_PROC_COUNT 7
#define DEFAULT_SPEED 0

/**
 * One for the library, four for me
 *
 * Semaphore overview:
 *   - SEMAPHORE_READY to wait for everybody
 *   - SEMAPHORE_ALL_SHOOTED is 0 when everyone has shooted
 *   - SEMAPHORE_ALL_RECEIVED is 0 when everyone has received his shot
 *   - SEMAPHORE_READY is used at the beginning and is 0 when all processes are ready
 *        We can't use this semaphore later, since one process can lock it, and without
 *        waiting for zero, the rest wake up
 */
#define TOTAL_SEMAPHORE_COUNT 4
#define SEMAPHORE_ALL_SHOOTED 1
#define SEMAPHORE_ALL_RECEIVED 2
#define SEMAPHORE_READY 2 // Reused
#define SEMAPHORE_LOG 3

/** Error */
#define ERROR(str, ...) do { \
	char buff[512]; \
	snprintf(buff, 512, str, ##__VA_ARGS__); \
	pon_error(buff); \
} while ( 0 )
/* #define ERROR(str, ...) do { \
	fprintf(stderr, "(error) "str, ## __VA_ARGS__); \
} while (0) */

#define ERROR_EXIT() exit(100)

#define FATAL_ERROR(str, ...) do { \
	ERROR(str, ##__VA_ARGS__); \
	fprintf(stderr, "Run with no args for a list of options.\n"); \
	ERROR_EXIT(); \
} while( 0 )

#define LOG_PATH "pist2.log"
#define LOG(msg, ...) do { \
	GameData* data = get_data(); \
	semaphore_wait(data->semaphores, SEMAPHORE_LOG); \
	if ( fseek(data->log, 0, SEEK_END) > -1 ) {  \
		fprintf(data->log, "("PID_T_FORMAT") "msg"\n", getpid(), ##__VA_ARGS__); \
		fflush(data->log); \
	} \
	semaphore_signal(data->semaphores, SEMAPHORE_LOG); \
} while (0)

#define LIMIT_MIN 3
#define LIMIT_MAX 26

/** To be a bit clearer */
typedef int semaphore_t;
typedef unsigned char bool;

#ifdef STATEMENT_CONFORMANT
	typedef int message_queue_t;

#	define MESSAGE_MAX_CONTENT_SIZE 10
#	define MESSAGE_DEAD_CONTENT "DEAD"

	typedef struct message {
		long type;
		char content[MESSAGE_MAX_CONTENT_SIZE];
	} message_t;
#endif

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
	unsigned short alive_count; /** Current round alive pids count */
	Process *children; /** The children processes */
	semaphore_t semaphores;
#ifdef STATEMENT_CONFORMANT
	message_queue_t message_queue;
#endif
	FILE* log;
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

/** Another helper. Instead of signal() (implementation dependent), we use sigaction */
#define BIND_TO(signal, handler) do { \
	struct sigaction action; \
	/** Block everything during execution */ \
	sigfillset(&action.sa_mask); \
	action.sa_flags = 0; \
	action.sa_handler = handler; \
	sigaction(signal, &action, NULL); \
} while (0)


/** Debugging and resource management */
void __dump();
void kill_all();
void sig_exit(int);
void release_all_data();
int semaphore_set_value(semaphore_t sem, unsigned short index, int val);
int semaphore_get_value(semaphore_t sem, unsigned short index);
int semaphore_change_value(semaphore_t sem, unsigned short index, short value);
int semaphore_wait_zero(semaphore_t sem, unsigned short index);
int semaphore_wait(semaphore_t sem, unsigned short index);
int semaphore_signal(semaphore_t sem, unsigned short index);

/** Utilities */
void program_help();
Process * current_proc();

/** Game logic */
void bind_parent_signals();
void bind_children_signals();
bool is_current_proc_coordinator();
int child_proc(char);

/** Main semaphore utilities */
int semaphore_set_value(semaphore_t sem, unsigned short index, int val) {
	union semun param;
	int ret;

	param.val = val;
	ret = semctl(sem, index, SETVAL, param);

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

/** Change the value atomically: increment or decrement */
int semaphore_change_value(semaphore_t sem, unsigned short index, short value) {
	static struct sembuf buff = { 0, 0, 0 };
	buff.sem_num = index;
	buff.sem_op = value;

	int ret = semop(sem, &buff, 1);
	if ( ret == -1 )
		FATAL_ERROR("semaphore_wait: %s", strerror(errno));
	return ret;
}

int semaphore_wait_zero(semaphore_t sem, unsigned short index) {
	return semaphore_change_value(sem, index, 0);
}

int semaphore_wait(semaphore_t sem, unsigned short index) {
	return semaphore_change_value(sem, index, -1);
}

int semaphore_signal(semaphore_t sem, unsigned short index) {
	return semaphore_change_value(sem, index, +1);
}

/** Dump data */
void __dump() {
	GameData *data = get_data();
	size_t i;
	Process *p;

	if ( ! data ) {
		fprintf(stderr, "No data available\n");
		return;
	}

	fprintf(stderr, "\nDump:\n");
	fprintf(stderr, "Alive count: %hu\n", data->alive_count);
	fprintf(stderr, "Semaphores:\n");
	for ( i = 0; i < TOTAL_SEMAPHORE_COUNT; ++i )
		fprintf(stderr, "\t%d", semaphore_get_value(data->semaphores, i));
	fprintf(stderr, "\n");

	EACH(data->children, data->process_count, p,
		fprintf(stderr, "{ "PID_T_FORMAT", %d}\t", p->id, p->status);
	);
	fprintf(stderr, "\n");
}

/** Program help */
void program_help() {
	fprintf(stderr, "Usage: "SELF" <count> <speed> <seed>\n");
	ERROR_EXIT();
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
	GameData* data = get_data();
	pid_t current_pid = getpid();
	Process* p;
	size_t i;

	i = data->process_count;
	while ( i-- ) {
		p = data->children + i;
		/** If there's a process which is not dead */
		if ( ~p->status & PID_STATUS_DEAD ) {
			if ( p->id == current_pid )
				return 1;
			else
				return 0;
		}
	}

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
		data->log = fopen(LOG_PATH, "w+");
		if ( data->log == NULL )
			data->log = stderr;
	}

	/** Release data */
	if ( action == DATA_RELEASE ) {
		if ( data->semaphores > -1 )
			semctl(data->semaphores, 0, IPC_RMID);

#ifdef STATEMENT_CONFORMANT
		if ( data->message_queue > -1 )
			msgctl(data->message_queue, IPC_RMID, NULL);
#endif

		if ( data->log != NULL )
			fclose(data->log);

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

/** Resources handled by atexit() */
void sig_exit(int s) {
	LOG("Signal %d received\n", s);
	__dump();
	ERROR_EXIT();
}

/** Our parent was interrupted, dump data for debugging and release all */
void release_all_resources() {
	GameData* data = get_data();
	int ret;

	/** Just for parent */
	if ( data && data->parent_id != getpid() )
		return;

	/** When game is over... */
	ret = PIST_fin();
	if ( ret == -1 )
		LOG("Library termination failed\n");

	kill_all();
	release_data();
	fflush(stderr);
	fclose(stderr);

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

	BIND_TO(SIGTERM, sig_exit);
	BIND_TO(SIGINT, sig_exit);
}

/** Child process subroutine */
int child_proc(char lib_id) {
	GameData* data = get_data();
	Process* me = current_proc();
	char target;
	int ret;
	bool im_coordinator;

	bind_children_signals();

	/** We update our status */
	me->status = PID_STATUS_READY;

	ret = PIST_nuevoPistolero(lib_id);
	if ( ret == -1 )
		FATAL_ERROR("Shooter not initialized");

	LOG("Ready to roll");
	/** Ensure we're ready, and wait for everybody else */
	semaphore_wait(data->semaphores, SEMAPHORE_READY);
	semaphore_wait_zero(data->semaphores, SEMAPHORE_READY);
	LOG("Loaded");

	/** S1 = 0; S2 = 0; */
	while ( 1 ) {
		/** Check if I'm the coordinator */
		im_coordinator = is_current_proc_coordinator();

		/** If I am, give pass to everyone two times: one for start and one for end */
		if ( im_coordinator ) {
			if ( data->alive_count == 1 )
				exit(0);
			data->rounds++;
			LOG("Round: %d, alive: %hu", data->rounds, data->alive_count);

			/** Increment semaphore 1 value alive_count times */
			semaphore_change_value(data->semaphores, 1, data->alive_count); // Allow everyone to shoot
		}
		LOG("Starting to shoot");

		target = PIST_vIctima();

		if ( target == '@' )
			FATAL_ERROR("This shit blew up\n");

		/** Mark the target as shot, equivalent to send the "DEAD" message */
		data->children[target - 'A'].status |= PID_STATUS_DEAD_THIS_ROUND;

#ifdef STATEMENT_CONFORMANT
		message_t msg;
		msg.type = target;
		strncpy(msg.content, MESSAGE_DEAD_CONTENT, MESSAGE_MAX_CONTENT_SIZE);
		if ( msgsnd(data->message_queue, &msg, MESSAGE_MAX_CONTENT_SIZE, 0) == -1 )
			FATAL_ERROR("msgsend: %s\n", strerror(errno));
#endif

		if ( PIST_disparar(target) == -1 )
			ERROR("PIST_disparar");

		semaphore_wait(data->semaphores, 1);
		LOG("Shot to %c", target);

		/** Wait for everyone to shoot */
		semaphore_wait_zero(data->semaphores, 1);

		if ( im_coordinator ) {
			/** When everyone has shooted, allow to receive, this has to be two times alive_count because they must be locked if everyone hasn't stopped */
			semaphore_change_value(data->semaphores, 2, data->alive_count * 2);
		}

		/** Without this lock, someone can shoot, reach the bottom, and start shooting again, or get its status corrupted */
		semaphore_wait(data->semaphores, 2);

#ifdef STATEMENT_CONFORMANT
		message_t message;
		/** This is far less elegant, but required given the practice statement */
		if ( msgrcv(data->message_queue, &message, MESSAGE_MAX_CONTENT_SIZE, lib_id, IPC_NOWAIT | MSG_NOERROR) != -1 ) {
			/** Empty the message queue */
			while ( msgrcv(data->message_queue, &message, MESSAGE_MAX_CONTENT_SIZE, lib_id, IPC_NOWAIT | MSG_NOERROR) != -1 ) { /* noop */ };

			/** Just some assertions, in case */
			assert(strcmp(message.content, MESSAGE_DEAD_CONTENT) == 0);
			assert(me->status & PID_STATUS_DEAD_THIS_ROUND);
#else
		/** If we have received a shot, mark as dead, else... we're ready */
		if ( me->status & PID_STATUS_DEAD_THIS_ROUND ) {
#endif
			me->status |= PID_STATUS_DEAD;
			ATOMIC_DECREMENT(&data->alive_count);
			PIST_morirme();
		} else {
			me->status = PID_STATUS_READY;
		}

		/**
		 * Wait until semaphore 2 is zero (everyone has died in the previous round)
		 * This wait is global because we can't choose a coordinator until everyone is
		 * dead
		 */
		semaphore_wait(data->semaphores, 2);
		semaphore_wait_zero(data->semaphores, 2);

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
		speed = DEFAULT_SPEED,
		seed = 0;
	pid_t current_pid,
		  last_dead_process;
	GameData *data;
	int ret;
	char lib_id = 'A';

	if ( argc < 3 || argc > 4 )
		program_help();

	if ( argv[1][0] >= '0' && argv[1][0] <= '9' )
		count = strtoul(argv[1], NULL, 10);
	else
		FATAL_ERROR("Option not recognized: %s", argv[1]);

	if ( argv[2][0] >= '0' && argv[2][0] <= '9' )
		speed = strtoul(argv[2], NULL, 10);
	else
		FATAL_ERROR("Option not recognized: %s", argv[2]);

	if ( argc == 4 ) {
		if ( argv[3][0] >= '0' && argv[3][0] <= '9' )
			 seed = strtoul(argv[3], NULL, 10);
		else
			FATAL_ERROR("Option not recognized: %s", argv[3]);
	}

#if CHECK_LIMITS
	if ( count < LIMIT_MIN || count > LIMIT_MAX )
		FATAL_ERROR("Number of processes must be between %d and %d.\n", LIMIT_MIN, LIMIT_MAX);
#endif

	if ( count == 0 )
		FATAL_ERROR("At least one player is required.\n");

	bind_parent_signals();

	atexit(release_all_resources);

	/** Create shared data structure */
	data = create_data(count);

	data->parent_id = getpid();

	data->semaphores = semget(IPC_PRIVATE, TOTAL_SEMAPHORE_COUNT, IPC_CREAT | 0600);

	if ( data->semaphores == -1 )
		FATAL_ERROR("Semaphore creation failed: %s\n", strerror(errno));

#ifdef STATEMENT_CONFORMANT
	data->message_queue = msgget(IPC_PRIVATE, IPC_CREAT | 0600);

	if ( data->message_queue == -1 )
		FATAL_ERROR("Message queue creation failed: %s\n", strerror(errno));
#endif

	ret = PIST_inicio(count, speed, data->semaphores, data->library_data, seed);

	if ( ret == -1 )
		FATAL_ERROR("Library initialization failed\n");


	/** Set the "ready" semaphore to `count`: we'll wait to 0 in the main proc to wait until they are all ready */
	semaphore_set_value(data->semaphores, SEMAPHORE_READY, count);

	/** Just one process logging at a time */
	semaphore_set_value(data->semaphores, SEMAPHORE_LOG, 1);

	/** Process creation */
	for ( i = 0; i < count; ++i ) {
		/** All processes start as dead */
		data->children[i].status = PID_STATUS_DEAD;
		current_pid = fork();
		switch ( current_pid ) {
			case -1:
				perror("fork");
				__dump();
				ERROR_EXIT();
			case 0:
				/** We must ensure current_index is ok in child_proc */
				data->children[i].id = getpid();
				return child_proc(lib_id);
		}
		lib_id++;
	}

	/** Wait for all processes to die */
	while ( count-- )
		last_dead_process = waitpid(0, NULL, 0);

	if ( data->alive_count == 1 )
		return last_dead_process;

	return 0;
}
