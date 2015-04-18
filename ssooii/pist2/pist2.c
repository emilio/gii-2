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

#ifndef BYPASS_STATEMENT
#	include <sys/msg.h>
#endif

#include "pist2.h"

union semun {
	int val;
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
};

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

// Default process count and output
#define DEFAULT_PROC_COUNT 7
#define DEFAULT_SPEED 0

// Semaphore overview:
//   - SEMAPHORE_READY to wait for everybody
//   - SEMAPHORE_ALL_SHOOTED will be zero when everyone has shooted
//   - SEMAPHORE_ALL_RECEIVED will be zero when everyone has received his shot
// We'll use one extra semaphore (SEMAPHORE_LOG) in case we're debugging.
// This extra semaphore acts as a mutex and will guard the log file
#ifdef DEBUG
#    define TOTAL_SEMAPHORE_COUNT 4
#    define SEMAPHORE_LOG 3
#else
#    define TOTAL_SEMAPHORE_COUNT 3
#endif
#define SEMAPHORE_ALL_SHOOTED 1
#define SEMAPHORE_ALL_RECEIVED 2
#define SEMAPHORE_READY 2 /* Reused */

#define ERROR(str, ...) do { \
	char buff[512]; \
	snprintf(buff, 512, str, ##__VA_ARGS__); \
	pon_error(buff); \
} while ( 0 )

#define ERROR_EXIT() exit(100)

#define FATAL_ERROR(str, ...) do { \
	ERROR(str, ##__VA_ARGS__); \
	fprintf(stderr, "Run with no args for a list of options.\n"); \
	ERROR_EXIT(); \
} while( 0 )

#ifdef DEBUG
#define LOG_PATH "pist2.log"
#define LOG(msg, ...) do { \
	GameData* data = get_data(); \
	semaphore_wait(data->semaphores, SEMAPHORE_LOG); \
	if ( fseek(data->log, 0, SEEK_END) > -1 ) {  \
		fprintf(data->log, "(%ld) "msg"\n", (long) getpid(), ##__VA_ARGS__); \
		fflush(data->log); \
	} \
	semaphore_signal(data->semaphores, SEMAPHORE_LOG); \
} while (0)
#else
#define LOG(...)
#endif

#define LIMIT_MIN 3
#define LIMIT_MAX 26

// Just some typedefs to be a bit clearer
typedef int semaphore_t;
typedef unsigned char bool;

// If we're literally following the statement we need some
// aditional definitions
#ifndef BYPASS_STATEMENT
	typedef int message_queue_t;

#	define MESSAGE_MAX_CONTENT_SIZE 5
#	define MESSAGE_DEAD_CONTENT "DEAD"

	typedef struct message {
		long type;
		char content[MESSAGE_MAX_CONTENT_SIZE];
	} message_t;
#endif

// We'll use an abstraction over a pid, called `Process`.
// This struct has a `status` field, which is a bitmask,
// in order to support mixed status.
//
// This is the bitmask itself.
typedef enum PidStatusType {
	PID_STATUS_READY = 0x01,
	PID_STATUS_SHOT = 0x02,
	PID_STATUS_DEAD_THIS_ROUND = 0x04,
	PID_STATUS_DEAD = 0x08
} PidStatusType;

typedef struct Process {
	pid_t id;
	PidStatusType status;
} Process;

// The main shared data structure.
// Members:
//  * process_count: Number of processes playing, gets incremented once while
//      creating processes, and never mutates again.
//  * parent_id: The parent process id
//  * rounds: The current round count
//  * alive_count: The currently alive process count
//  * children: The dinamically allocated array of children processes
//  * semaphores: The main reference to our semaphores
//  * message_queue: The message queue used to shoot if we follow strictly
//      the statement.
//  * log: The pointer to the log file, just if we're debugging.
//  * library_data: Shared data used by the library for god knows what.
typedef struct GameData {
	size_t process_count; /** Number of processes playing. This is inmutable */
	pid_t parent_id; /** The parent process id */
	size_t rounds; /** The round count */
	unsigned short alive_count; /** Current round alive pids count */
	Process *children; /** The children processes */
	semaphore_t semaphores;
#ifndef BYPASS_STATEMENT
	message_queue_t message_queue;
#endif
#ifdef DEBUG
	FILE* log;
#endif
	char library_data[256];
} GameData;

// In order to not use a single global variable, we must use `static` variable
// insid of a function which will take care of it. The function must support
// creating, retrieving and releasing the data.
typedef enum DataActionType {
	DATA_CREATE,
	DATA_REFRESH,
	DATA_GET,
	DATA_RELEASE
} DataActionType;


/* Debugging and resource management
 * ================================= */

// Dumps data to stderr, useful for debugging
void __dump();

// Kills all children processes, sending SIGKILL
void kill_all();

// Signal handler to exit the program, used by SIGINT and SIGTERM
void sig_exit(int);

// Release all shared resources
void release_all_resources();

// The function used to manage the data
GameData * manage_data(DataActionType, size_t);

// Calculates the required size of the shared fragment to hold `count` processes
#define DATA_SIZE_FOR(count) (sizeof(GameData) + sizeof(Process) * count)

// Some helper macros
#define get_data() manage_data(DATA_GET, 0)
#define release_data() manage_data(DATA_RELEASE, 0)
#define create_data(process_count) manage_data(DATA_CREATE, process_count)

/* Sychronization
 * ================================= */

// Semaphore helpers
int semaphore_set_value(semaphore_t sem, unsigned short index, int val);
int semaphore_get_value(semaphore_t sem, unsigned short index);
int semaphore_change_value(semaphore_t sem, unsigned short index, short value);
int semaphore_wait_zero(semaphore_t sem, unsigned short index);
int semaphore_wait(semaphore_t sem, unsigned short index);
int semaphore_signal(semaphore_t sem, unsigned short index);

// Bind a signal to a signal handler. We block all incoming signals while the
// handler is executed
#define BIND_TO(signal, handler) do { \
	struct sigaction action; \
	sigfillset(&action.sa_mask); \
	action.sa_flags = 0; \
	action.sa_handler = handler; \
	sigaction(signal, &action, NULL); \
} while (0)

// Signal handling
void block_all_signals();
void bind_and_unblock_parent_signals();

/* Game logic
 * ================================= */

// Prints program instructions
void program_help();

// Check if the current process is the coordinator of the game round
bool is_current_proc_coordinator();

// Children round logic
int child_proc(char);

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
		FATAL_ERROR("semaphore_change_value (%d, %d, %d): %s", sem, index, value, strerror(errno));

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

	if ( ! data ) {
		fprintf(stderr, "No data available\n");
		return;
	}

	fprintf(stderr, "\nDump:\n");
	fprintf(stderr, "Alive count: %hu\n", data->alive_count);
	fprintf(stderr, "Semaphores:\n");
	if ( data->semaphores > -1 ) {
		for ( i = 0; i < TOTAL_SEMAPHORE_COUNT; ++i )
			fprintf(stderr, "\t%d", semaphore_get_value(data->semaphores, i));
		fprintf(stderr, "\n");
	}

	for ( i = 0; i < data->process_count; ++i )
		fprintf(stderr, "{ %ld, %d}\t", (long) data->children[i].id, data->children[i].status);
	fprintf(stderr, "\n");
}

/** Program help */
void program_help() {
	fprintf(stderr, "Usage: ./pist2 <count> <speed> <seed>\n");
	ERROR_EXIT();
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

	assert(0); /* Unreachable */
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
		data->process_count = 0;
		data->alive_count = 0;
		data->parent_id = 0;
		data->rounds = 0;
		data->semaphores = -1;
#ifndef BYPASS_STATEMENT
		data->message_queue = -1;
#endif
		/** Adjust the pointer to the end of the struct */
		data->children = (Process *) (data + 1);
#ifdef DEBUG
		data->log = fopen(LOG_PATH, "w+");
		if ( data->log == NULL )
			data->log = stderr;
#endif
	}

	/** Release data */
	if ( action == DATA_RELEASE && data != NULL ) {
		union semun dummy;

		LOG("Releasing shared memory");

		if ( data->semaphores > -1 )
			if ( semctl(data->semaphores, 0, IPC_RMID, dummy) == -1 )
				LOG("Failed to remove semaphores: %s", strerror(errno));

#ifndef BYPASS_STATEMENT
		if ( data->message_queue > -1 )
			if ( msgctl(data->message_queue, IPC_RMID, NULL) == -1 )
				LOG("Failed to remove message queue: %s", strerror(errno));
#endif
#ifdef DEBUG
		if ( data->log != NULL )
			fclose(data->log);
#endif

		munmap( (void *) data, size);
		data = NULL;
	}

	return data;
}

/** Kill all pending processes */
void kill_all() {
	GameData *data = get_data();
	size_t i;

	if ( ! data )
		return;

	LOG("Killing all remaining processes");

	for ( i = 0; i < data->process_count; ++ i)
		if ( data->children[i].id /** If we kill uninitialized memory we may kill ourselves with 0 */
			 && kill(data->children[i].id, SIGKILL) == -1 )
			LOG("Kill to %ld failed: %s", (long) data->children[i].id, strerror(errno));
}

/** Our parent was interrupted, dump data for debugging and release everything */
void sig_exit(int s) {
	LOG("Signal %d received\n", s);
	__dump();
	ERROR_EXIT();
}

void release_all_resources() {
	GameData* data = get_data();
	int ret;

	/** Just for parent */
	if ( data && data->parent_id != getpid() )
		return;

	LOG("Releasing data");

	/** When game is over... */
	ret = PIST_fin();
	if ( ret == -1 )
		LOG("Library termination failed\n");

	kill_all();

	LOG("Calling `release_data`");
	release_data();
	fclose(stderr);

}

void block_all_signals() {
	sigset_t set;

	sigfillset(&set);
	sigprocmask(SIG_BLOCK, &set, NULL);
}

void bind_and_unblock_parent_signals() {
	sigset_t set;

	BIND_TO(SIGTERM, sig_exit);
	BIND_TO(SIGINT, sig_exit);

	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGTERM);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
}

/** Child process subroutine */
int child_proc(char lib_id) {
	GameData* data = get_data();
	Process* me = data->children + (lib_id - 'A');
	char target;
	int ret;
	bool im_coordinator;
	unsigned short this_round_alive_count = 0;

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
		LOG("Starting round");
		/** Check if I'm the coordinator */
		im_coordinator = is_current_proc_coordinator();

		/** If I am, give pass to everyone two times: one for start and one for end */
		if ( im_coordinator ) {
			if ( data->alive_count == 1 )
				exit(0);
			data->rounds++;

			/** We cache this value locally because it may change after the W0(S1) */
			this_round_alive_count = data->alive_count;
			LOG("Round: %d, alive: %hu", data->rounds, this_round_alive_count);

			/** Increment semaphore 1 value alive_count times */
			semaphore_change_value(data->semaphores,
                                   SEMAPHORE_ALL_SHOOTED,
                                   this_round_alive_count);
			LOG("Semaphore 1 increased");
		}
		LOG("Starting to shoot");

		target = PIST_vIctima();

		if ( target == '@' )
			FATAL_ERROR("This shit blew up\n");

		/** Mark the target as shot, equivalent to send the "DEAD" message */
		data->children[target - 'A'].status |= PID_STATUS_DEAD_THIS_ROUND;

#ifndef BYPASS_STATEMENT
		message_t msg;
		msg.type = target;
		strncpy(msg.content, MESSAGE_DEAD_CONTENT, MESSAGE_MAX_CONTENT_SIZE);
		if ( msgsnd(data->message_queue, &msg,
                    MESSAGE_MAX_CONTENT_SIZE, 0) == -1 )
			FATAL_ERROR("msgsend: %s\n", strerror(errno));
#endif

		if ( PIST_disparar(target) == -1 )
			ERROR("PIST_disparar");

		LOG("Shot to %c", target);

		semaphore_wait(data->semaphores, SEMAPHORE_ALL_SHOOTED);

		// Wait until everyone else has shooted
		semaphore_wait_zero(data->semaphores, SEMAPHORE_ALL_SHOOTED);

        // Increment the second semaphore to allow going to the next
        // round once all have received the shoot
		if ( im_coordinator ) {
			semaphore_change_value(data->semaphores,
                                   SEMAPHORE_ALL_RECEIVED,
                                   this_round_alive_count);
		}

#ifndef BYPASS_STATEMENT
		message_t message;
		// This is far less elegant, but required given the
		// practice statement:
		//
		// We check for messages in the queue, and if we find them
		// empty the queue.
		if ( msgrcv(data->message_queue, &message,
                    MESSAGE_MAX_CONTENT_SIZE, lib_id,
                    IPC_NOWAIT | MSG_NOERROR) != -1 ) {
			while ( msgrcv(data->message_queue, &message,
                           MESSAGE_MAX_CONTENT_SIZE, lib_id,
                           IPC_NOWAIT | MSG_NOERROR) != -1 ) { /* noop */ };

			// Just some assertions, in case
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

		// Wait until SEMAPHORE_ALL_RECEIVED is zero
		// (everyone has died in the previous round)
		//
		// This wait is global because we can't choose
		// a coordinator until everyone is marked as dead
		semaphore_wait(data->semaphores, SEMAPHORE_ALL_RECEIVED);
		semaphore_wait_zero(data->semaphores, SEMAPHORE_ALL_RECEIVED);

		// Just die (really if we should)
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

	// Easy resource deallocation
	atexit(release_all_resources);

	// We won't unblock parent signals until we've created all the resources.
	// Note children processes have all signals blocked, they will be SIGKILLed
	// by the parent.
	block_all_signals();

	/** Create shared data structure */
	data = create_data(count);

	data->parent_id = getpid();

	data->semaphores = semget(IPC_PRIVATE, TOTAL_SEMAPHORE_COUNT, IPC_CREAT | 0600);

	if ( data->semaphores == -1 )
		FATAL_ERROR("Semaphore creation failed: %s\n", strerror(errno));

#ifndef BYPASS_STATEMENT
	data->message_queue = msgget(IPC_PRIVATE, IPC_CREAT | 0600);

	if ( data->message_queue == -1 )
		FATAL_ERROR("Message queue creation failed: %s\n", strerror(errno));
#endif

	ret = PIST_inicio(count, speed, data->semaphores, data->library_data, seed);

	if ( ret == -1 )
		FATAL_ERROR("Library initialization failed\n");


	// Set the "ready" semaphore to `count + 1`:
	// This allow the parent to lazily increment `alive_count`
	// and `process_count`, since children won't read them
	// until parent has waited to the semaphore
	semaphore_set_value(data->semaphores, SEMAPHORE_READY, count + 1);
	semaphore_set_value(data->semaphores, SEMAPHORE_ALL_SHOOTED, 0);

#ifdef DEBUG
	// Just one process logging at a time
    semaphore_set_value(data->semaphores, SEMAPHORE_LOG, 1);
#endif

	 // Incrementing these on the fly allows correct
	 // resource deallocation in case a `fork` fails
	 //
	 // Note that a signal handler can't kill us until all processes
	 // are created
	data->process_count = data->alive_count = 0;

	/** Process creation */
	for ( i = 0; i < count; ++i ) {
		data->children[i].status = PID_STATUS_DEAD;
		current_pid = fork();
		switch ( current_pid ) {
			case -1:
				perror("fork");
				__dump();
				ERROR_EXIT();
			case 0:
				data->children[i].id = getpid();
				return child_proc(lib_id);
		}
		// Can't increment process_count
		// without being sure this is initialized
		data->children[i].id = current_pid;
		data->process_count++;
		data->alive_count++;
		lib_id++;
	}

	// Allow receiving sigint/sigterm, now resource deallocation is safe
	bind_and_unblock_parent_signals();

	// The parent does not need to wait until children are ready,
	// so no semaphore_wait_zero below
	semaphore_wait(data->semaphores, SEMAPHORE_READY);


	/** Wait for all processes to die */
	while ( count-- )
		last_dead_process = waitpid(0, NULL, 0);

	if ( data->alive_count == 1 )
		return last_dead_process;

	return 0;
}
