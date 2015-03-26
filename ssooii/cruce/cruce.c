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

#include "cruce.h"

union semun {
	int val;
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
};

#define SELF "cruce"

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
#		define ATOMIC_DECREMENT(var) (atomic_dec_32_nv(var))
#		define CAS(var, val_1, val_2) (atomic_cas_32(var, val_1, val_2))
#	else
#		define ATOMIC_DECREMENT(var) (__sync_sub_and_fetch(var, 1))
#       define CAS(var, val_1, val_2) (__sync_val_compare_and_swap(var, val_1, val_2))
#	endif
#elif _WIN32
#   define ATOMIC_DECREMENT(var) (InterlockedDecrement(var))
#	define CAS(var, val_1, val_2) (InterlockedCompareExchange(var, val_1, val_2)
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
#define SEMAPHORE_MAP_ACCESS 1
#define SEMAPHORE_MANAGER_READY 2
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

#define FATAL_ERROR(str, ...) do { \
	ERROR(str, ##__VA_ARGS__); \
	fprintf(stderr, "Run with no args for a list of options.\n"); \
	exit(1); \
} while( 0 )

#define LOG_PATH SELF".log"
#define LOG(msg, ...) do { \
	GameData* data = get_data(); \
	semaphore_lock(data->semaphores, SEMAPHORE_LOG); \
	if ( fseek(data->log, 0, SEEK_END) > -1 ) {  \
		fprintf(data->log, "("PID_T_FORMAT") "msg"\n", getpid(), ##__VA_ARGS__); \
		fflush(data->log); \
	} \
	semaphore_unlock(data->semaphores, SEMAPHORE_LOG); \
} while (0)

#define LIMIT_MIN 3
#define LIMIT_MAX 26

/** To be a bit clearer */
typedef int semaphore_t;
typedef unsigned char bool;
typedef struct posiciOn point_t;
#define CAR COCHE
#define WALKER PEAToN

/** The  action type passed to manage_data */
typedef enum DataActionType {
	DATA_CREATE,
	DATA_REFRESH,
	DATA_GET,
	DATA_RELEASE
} DataActionType;

/** Process struct */
typedef struct Process {
	pid_t id; /** PID */
} Process;

typedef struct Semaphore {
	point_t range[2];
	int color;
} Semaphore;

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
	/* const */ pid_t manager_id;
	size_t rounds; /** The round count */
	int alive_count; /** Current round alive pids count */
	Process *children; /** The children processes */
	int grid[1024][1024];
	Semaphore game_semaphores[4];
	semaphore_t semaphores;
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
		FATAL_ERROR("semaphore_lock: %s", strerror(errno));
	return ret;
}

/** Wait zero */
int semaphore_wait_zero(semaphore_t sem, unsigned short index) {
	return semaphore_change_value(sem, index, 0);
}

/** Equivalent to `wait` */
int semaphore_lock(semaphore_t sem, unsigned short index) {
	return semaphore_change_value(sem, index, -1);
}

/** Equivalent to `signal` */
int semaphore_unlock(semaphore_t sem, unsigned short index) {
	return semaphore_change_value(sem, index, +1);
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

/** Parent process logic */
int parent_proc();

/** Dump data */
void __dump() {
	GameData *data = get_data();
	size_t i;
	Process *p;

	printf("\nDump:\n");
	printf("Alive count: %hu\n", data->alive_count);
	printf("Semaphores:\n");
	for ( i = 0; i < TOTAL_SEMAPHORE_COUNT; ++i )
		printf("\t%d", semaphore_get_value(data->semaphores, i));
	printf("\n");

	EACH(data->children, data->process_count, p,
		printf("{ "PID_T_FORMAT"}\t", p->id);
	);
	printf("\n");
}

/** Program help */
void program_help() {
	printf("Usage: "SELF" <count> <speed> <seed>\n");
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
		data->alive_count = 0;
		data->parent_id = 0;
		data->rounds = 0;
		data->semaphores = 0;
		memset(data->grid, 0, sizeof(data->grid));
		/** Adjust the pointer to the end of the struct, and ensure it's 0 */
		data->children = (Process *) (data + 1);
		memset(data->children, 0, sizeof(Process) * count);
		data->log = fopen(LOG_PATH, "w+");
		if ( data->log == NULL )
			data->log = stderr;
	}

	/** Release data */
	if ( action == DATA_RELEASE ) {
		/** TODO: Is count ignored in semctl with RMID? */
		if ( data->semaphores > 0 )
			semctl(data->semaphores, 0, IPC_RMID);

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

	kill(data->manager_id, SIGKILL);

	EACH(data->children, data->process_count, p,
		kill(p->id, SIGKILL);
	);
}

/** Resources handled by atexit() */
void sig_exit(int s) {
	__dump();
	exit(1);
}

/** Our parent was interrupted, dump data for debugging and release all */
void release_all_resources() {
	GameData* data = get_data();
	int ret;

	/** Just for parent */
	if ( data->parent_id != getpid() )
		return;

	/** When game is over... */
	ret = CRUCE_fin();
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

/** Helper macro to hash a location to the grid table: */
#define GRID_VALUE_FOR(p) (data->grid[p.x + 3][p.y + 3])

int child_proc_walker(size_t my_index) {
	GameData* data = get_data();
	Process* me = data->children + my_index;
	point_t current = {0, 0},
			next;
	int ret;

	LOG("Walker children started with index %zu", my_index);

	bind_children_signals();

	next = CRUCE_inicio_peatOn_ext(&current);

	LOG("Walker current: (%d, %d), next: (%d, %d)", current.x, current.y, next.x, next.y);

	semaphore_lock(data->semaphores, SEMAPHORE_MAP_ACCESS);
	GRID_VALUE_FOR(current) = 1;
	semaphore_unlock(data->semaphores, SEMAPHORE_MAP_ACCESS);

	while ( next.y >= 0 ) {
		ret = pausa();
		if ( ret == -1 ) {
			LOG("process pause fail, dying");
			break;
		}

		semaphore_lock(data->semaphores, SEMAPHORE_MAP_ACCESS);
		if ( ! GRID_VALUE_FOR(next) ) {
			LOG("Moving from (%d, %d) to (%d, %d)", current.x, current.y, next.x, next.y);
			GRID_VALUE_FOR(current) = 0;
			GRID_VALUE_FOR(next) = 1;
			current = next;
			next = CRUCE_avanzar_peatOn(next);
			LOG("Avanzar returned (%d, %d)", next.x, next.y);
		}
		semaphore_unlock(data->semaphores, SEMAPHORE_MAP_ACCESS);
	}

	semaphore_lock(data->semaphores, SEMAPHORE_MAP_ACCESS);
	GRID_VALUE_FOR(current) = 0;
	LOG("Walker exiting");
	CRUCE_fin_peatOn();
	me->id = 0;
	semaphore_unlock(data->semaphores, SEMAPHORE_MAP_ACCESS);
	return 0;
}

int child_proc_car(size_t my_index) {
	GameData* data = get_data();
	Process* me = data->children + my_index;
	point_t current,
			next;
	int ret;

	LOG("Car children started with index %zu", my_index);

	bind_children_signals();

	current = next = CRUCE_inicio_coche();

	/** Wait for the semaphore to be ready */
	semaphore_wait_zero(data->semaphores, SEMAPHORE_MANAGER_READY);

	LOG("Car children loop start (%d, %d)", current.x, current.y);
	while ( next.y >= 0 ) {
		ret = pausa_coche();
		if ( ret == -1 ) {
			LOG("process pause fail, dying");
			break;
		}
		semaphore_lock(data->semaphores, SEMAPHORE_MAP_ACCESS);
		LOG("Car checking (%d, %d): %d", next.x, next.y, GRID_VALUE_FOR(next));
		if ( ! GRID_VALUE_FOR(next) ) {
			LOG("Car moving from (%d, %d) to (%d, %d)", current.x, current.y, next.x, next.y);
			GRID_VALUE_FOR(current) = 0;
			GRID_VALUE_FOR(next) = 1;

			current = next;
			next = CRUCE_avanzar_coche(next);
			LOG("Car avanzar returned (%d, %d)", next.x, next.y);
		}
		semaphore_unlock(data->semaphores, SEMAPHORE_MAP_ACCESS);
	}

	semaphore_lock(data->semaphores, SEMAPHORE_MAP_ACCESS);
	GRID_VALUE_FOR(current) = 0;
	semaphore_unlock(data->semaphores, SEMAPHORE_MAP_ACCESS);

	LOG("Car exiting");
	CRUCE_fin_coche();
	me->id = 0;
	return 0;
}

#define INIT_RANGE(sem, p1x, p1y, p2x, p2y) do { \
	point_t* range = data->game_semaphores[sem].range;	\
	range[0].x = p1x; \
	range[0].y = p1x; \
	range[1].x = p2x; \
	range[1].y = p2y; \
} while (0)
int manager_proc() {
	GameData* data = get_data();
	int phase = 0;

	INIT_RANGE(SEM_C1, 12, 4, 16, 7);

	data->game_semaphores[SEM_C1].range = {{0, 0}, {1, 1}};
	CRUCE_pon_semAforo(SEM_C1, VERDE);
	CRUCE_pon_semAforo(SEM_P2, VERDE);

	CRUCE_pon_semAforo(SEM_P1, ROJO);
	CRUCE_pon_semAforo(SEM_C2, ROJO);

	CRUCE_pon_semAforo(SEM_C1, VERDE);
	CRUCE_pon_semAforo(SEM_P2, VERDE);
	CRUCE_pon_semAforo(SEM_P1, VERDE);
	CRUCE_pon_semAforo(SEM_C2, VERDE);

	/** Tell everyone we're ready */
	semaphore_unlock(data->semaphores, SEMAPHORE_MANAGER_READY);

	while(1) pausa();

	while ( 1 ) {
		if ( phase == 6 ) {
			semaphore_lock(data->semaphores, SEMAPHORE_MAP_ACCESS);
			CRUCE_pon_semAforo(SEM_C2, VERDE);
			CRUCE_pon_semAforo(SEM_C1, AMARILLO);

			CRUCE_pon_semAforo(SEM_P1, ROJO);
			CRUCE_pon_semAforo(SEM_P2, ROJO);
			semaphore_unlock(data->semaphores, SEMAPHORE_MAP_ACCESS);
		} if ( phase == 8 ) {
			CRUCE_pon_semAforo(SEM_C1, ROJO);
		} else if ( phase == 14 ) {
			semaphore_lock(data->semaphores, SEMAPHORE_MAP_ACCESS);
			CRUCE_pon_semAforo(SEM_C2, VERDE);
			CRUCE_pon_semAforo(SEM_C1, ROJO);

			CRUCE_pon_semAforo(SEM_P1, ROJO);
			CRUCE_pon_semAforo(SEM_P2, ROJO);
			semaphore_unlock(data->semaphores, SEMAPHORE_MAP_ACCESS);
		}
		phase++;
		pausa();
	}
}

/** Main process logic */
int main(int argc, char **argv) {
	size_t i = 0,
		max_count = DEFAULT_PROC_COUNT,
		speed = DEFAULT_SPEED;
	pid_t current_pid;
	GameData *data;
	int ret,
		type;

	if ( argc < 3 || argc > 4 )
		program_help();

	if ( argv[1][0] >= '0' && argv[1][0] <= '9' )
		max_count = strtoul(argv[1], NULL, 10);
	else
		FATAL_ERROR("Option not recognized: %s", argv[1]);

	if ( argv[2][0] >= '0' && argv[2][0] <= '9' )
		speed = strtoul(argv[2], NULL, 10);
	else
		FATAL_ERROR("Option not recognized: %s", argv[2]);

#if CHECK_LIMITS
	if ( max_count < LIMIT_MIN || max_count > LIMIT_MAX )
		FATAL_ERROR("Number of processes must be between %d and %d.\n", LIMIT_MIN, LIMIT_MAX);
#endif

	if ( max_count == 0 )
		FATAL_ERROR("At least one player is required.\n");

	atexit(release_all_resources);

	max_count -= 2; // One for manager, one for parent

	/** Create shared data structure */
	data = create_data(max_count);

	data->parent_id = getpid();

	/** Another for us */
	data->semaphores = semget(IPC_PRIVATE, TOTAL_SEMAPHORE_COUNT, IPC_CREAT | 0600);

	if ( data->semaphores == -1 )
		FATAL_ERROR("Semaphore creation failed: %s\n", strerror(errno));

	ret = CRUCE_inicio(speed, max_count + 2, data->semaphores, data->library_data);

	if ( ret == -1 )
		FATAL_ERROR("Library initialization failed\n");

	bind_parent_signals();

	semaphore_set_value(data->semaphores, SEMAPHORE_MAP_ACCESS, 1);
	semaphore_set_value(data->semaphores, SEMAPHORE_LOG, 1);
	semaphore_set_value(data->semaphores, SEMAPHORE_MANAGER_READY, 0);

	current_pid = fork();

	switch ( current_pid ) {
		case -1:
			perror("manager fork");
			exit(1);
		case 0:
			return manager_proc();
	}

	data->manager_id = current_pid;

	LOG("Manager started");

	semaphore_lock(data->semaphores, SEMAPHORE_MANAGER_READY);


	/** Process creation */
	while ( 1 ) {
		if ( data->alive_count == max_count ) {
			waitpid(-1, NULL, 0);
			data->alive_count--;
		}

		for ( i = 0; i < max_count; ++i )
			if ( data->children[i].id == 0 )
				break;

		if ( i == max_count )
			continue;

		LOG("Adding child with index %zu", i);

		type = CRUCE_nuevo_proceso();
		current_pid = fork();
		switch ( current_pid ) {
			case -1:
				perror("fork");
				__dump();
				exit(1);
			case 0:
				data->children[i].id = getpid();
				switch ( type ) {
					case WALKER:
						return child_proc_walker(i);
					case CAR:
						return child_proc_car(i);
				}
		}
		data->children[i].id = current_pid;
		data->alive_count++;
		pausa();
	}

	return 0;
}
