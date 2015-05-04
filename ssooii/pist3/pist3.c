/**
 * Pistolo.c
 *
 * http://avellano.usal.es/~ssooi/pract115.htm
 *
 * @author Emilio Cobos Álvarez <emiliocobos@usal.es>
 */
#define _UNICODE
#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* strlen, strerror */
#include <time.h> /* time */
#include <windows.h>
#define ERROR_EXIT() exit(100)

#define ERROR_MSG(str, ...) do { \
	fprintf(stderr, str "\n", ##__VA_ARGS__); \
} while (0)

#define FATAL_ERROR_MSG(str, ...) do { \
	LPVOID lpErrBuffer; \
	DWORD dwErrCode; \
	ERROR_MSG(str, ##__VA_ARGS__); \
   	dwErrCode = GetLastError(); \
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, \
	              NULL, \
	              dwErrCode, \
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
	              (LPSTR) &lpErrBuffer, \
	              0, NULL); \
	fprintf(stderr, TEXT("Windows Error: %s\n"), lpErrBuffer); \
	ERROR_EXIT(); \
} while( 0 )

#define LOG_PATH "pist3.log"
HANDLE logMutex;
#define LOG(msg, ...) do { \
	GameData* data = get_data(); \
	DWORD dwWaitResult = WaitForSingleObject(logMutex, 0L);	\
	if ( dwWaitResult == WAIT_OBJECT_0 ) { \
		if ( fseek(data->log, 0, SEEK_END) > -1 ) { \
			fprintf(data->log, "(%d) "msg"\n", GetCurrentThreadId(), ##__VA_ARGS__); \
        } \
        fflush(data->log); \
		ReleaseMutex(logMutex); \
	} \
} while (0)

#define LIMIT_MIN 3
#define LIMIT_MAX 26
#define MAX_CHILDREN LIMIT_MAX
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
} StatusType;

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
	/* const */ HANDLE parent_thread; /** The parent process id */
	size_t rounds; /** The round count */
	unsigned short alive_count; /** Current round alive pids count */
	HANDLE threads[MAX_CHILDREN];
	StatusType statuses[MAX_CHILDREN]; /** The children processes */
	HANDLE semaphore;
    HANDLE ready_event;
	FILE* log;
} GameData;

/** Get the main game data struct, create if non-zero value is passed as a second argument */
GameData * manage_data(DataActionType, size_t);

#define DATA_SIZE_FOR(count) (sizeof(GameData) + sizeof(Process) * count)

/** Wrapper macros for GameData actions */
#define get_data() manage_data(DATA_GET, 0)
#define refresh_data() manage_data(DATA_REFRESH, 0)
#define release_data() manage_data(DATA_RELEASE, 0)
#define create_data(process_count) manage_data(DATA_CREATE, process_count)

struct lib {
       void (*init)(int, int, int);
       int (*newShooter)(char);
       char (*victim)(void);
       int (*shoot)(char);
       void (*die)(void);
       int (*deinit)(void);
       void (*error)(char*);
} lib;

bool init_lib() {
    HINSTANCE dlllib = LoadLibrary("pist3.dll");
    if ( ! dlllib )
       return false;
    lib.init = (void (*)(int, int, int)) GetProcAddress(dlllib, "PIST_inicio");
    lib.newShooter = (int (*) (char)) GetProcAddress(dlllib, "PIST_nuevoPistolero");
    lib.victim = (char (*) (void)) GetProcAddress(dlllib, "PIST_vIctima");
    lib.shoot = (int(*)(char)) GetProcAddress(dlllib, "PIST_disparar");
    lib.die = (void (*) (void)) GetProcAddress(dlllib, "PIST_morirme");
    lib.deinit = (int (*) (void)) GetProcAddress(dlllib, "PIST_fin");
    lib.error = (void (*)(char*)) GetProcAddress(dlllib, "pon_error");

    if ( lib.init == NULL ||
         lib.newShooter == NULL ||
         lib.victim == NULL ||
         lib.shoot == NULL ||
         lib.die == NULL ||
         lib.deinit == NULL ||
         lib.error == NULL )
        return false;

    return true;
}

typedef HANDLE semaphore_t;
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

/** Game logic */
void bind_parent_signals();
void bind_children_signals();
bool is_current_proc_coordinator();
int child_proc(char);

/** Change the value atomically: increment or decrement */
bool semaphore_change_value(semaphore_t sem, short value) {
    if ( value < 0 ) {
        if ( value != -1 ) {
            FATAL_ERROR_MSG("Multi-decrement not supported by windows");
        } else {
            return WaitForSingleObject(sem, 0L) == WAIT_OBJECT_0;
        }
    }

    return true;
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

	fprintf(stderr, "\nDump:\n");
	fprintf(stderr, "Alive count: %hu\n", data->alive_count);
}

/** Program help */
void program_help() {
	fprintf(stderr, "Usage: ./pist3 <count> <speed> <seed>\n");
	ERROR_EXIT();
}

bool is_current_proc_coordinator(size_t current_index) {
	GameData* data = get_data();
	size_t i;

	i = data->process_count;
	while ( i-- ) {
		/** If there's a process which is not dead */
		if ( ~data->statuses[i] & PID_STATUS_DEAD ) {
			if ( i == current_index )
				return 1;
			else
				return 0;
		}
	}

	return 0;
}

/** Manage game data */
GameData* manage_data(DataActionType action, size_t count) {
	static GameData* data = NULL;

	/** Create data */
	if ( action == DATA_CREATE && count != 0 && data == NULL ) {
        data = (GameData*) malloc(sizeof(GameData)); 
		data->process_count = count;
		data->alive_count = count;
		data->parent_thread = NULL;
		data->rounds = 0;
		data->semaphore = NULL;

		data->log = fopen(LOG_PATH, "w+");
		if ( !data->log )
			data->log = stderr;
	}

	/** Release data */
	if ( action == DATA_RELEASE ) {
		/** TODO: Is count ignored in semctl with RMID? */
		if ( data->semaphore )
		    if ( !CloseHandle(data->semaphore) )
                LOG("Release semaphore failed: %s\n", GetLastError());

		if ( data->log )
			fclose(data->log);

		free( (void *) data);
		data = NULL;
	}

	return data;
}

/** Kill all pending processes */
void kill_all() {
	GameData *data = get_data();

	for (size_t i = 0; i < data->process_count; ++i )
	    TerminateThread(data->threads[i], 1);
}

/** Resources handled by atexit() */
void sig_exit(int s) {
	__dump();
	ERROR_EXIT();
}

/** Our parent was interrupted, dump data for debugging and release all */
void release_all_resources() {
	GameData* data = get_data();
	int ret;

	/** When game is over... */
	ret = lib.deinit();
	if ( ret == -1 )
		LOG("Library termination failed\n");

	kill_all();
	release_data();
	fflush(stderr);
	fclose(stderr);

}

/** Child process subroutine */
int child_proc(void* my_status_ptr_) {
	GameData* data = get_data();
	size_t my_index = ((StatusType*)my_status_ptr_) - (StatusType*)data->statuses;
    char lib_id = 'A' + my_index;
	char target;
	int ret;
	bool im_coordinator;

	/** We update our status */
	data->statuses[my_index] = PID_STATUS_READY;

	ret = lib.newShooter(lib_id);
	if ( ret == -1 )
		FATAL_ERROR_MSG("Shooter not initialized");

	LOG("Ready to roll");
	// TODO: add load rendezvous here

	/** S1 = 0; S2 = 0; */
	while ( 1 ) {
		/** Check if I'm the coordinator */
		im_coordinator = is_current_proc_coordinator(my_index);

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

		target = lib.victim();

		if ( target == '@' )
			FATAL_ERROR_MSG("This shit blew up\n");

		/** Mark the target as shot, equivalent to send the "DEAD" message */
		data->statuses[target - 'A'] |= PID_STATUS_DEAD_THIS_ROUND;

		if ( lib.shoot(target) == -1 )
			ERROR_MSG("PIST_disparar");

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

		/** If we have received a shot, mark as dead, else... we're ready */
		if ( data->statuses[i] & PID_STATUS_DEAD_THIS_ROUND ) {
			data->statuses[i] |= PID_STATUS_DEAD;
			ATOMIC_DECREMENT(&data->alive_count);
			lib.die();
		} else {
			data->statuses[i] = PID_STATUS_READY;
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
		FATAL_ERROR_MSG("Option not recognized: %s", argv[1]);

	if ( argv[2][0] >= '0' && argv[2][0] <= '9' )
		speed = strtoul(argv[2], NULL, 10);
	else
		FATAL_ERROR_MSG("Option not recognized: %s", argv[2]);

	if ( argc == 4 ) {
		if ( argv[3][0] >= '0' && argv[3][0] <= '9' )
			 seed = strtoul(argv[3], NULL, 10);
		else
			FATAL_ERROR_MSG("Option not recognized: %s", argv[3]);
	}

#if CHECK_LIMITS
	if ( count < LIMIT_MIN || count > LIMIT_MAX )
		FATAL_ERROR_MSG("Number of processes must be between %d and %d.\n", LIMIT_MIN, LIMIT_MAX);
#endif

	if ( count == 0 )
		FATAL_ERROR_MSG("At least one player is required.\n");

	atexit(release_all_resources);

    if ( ! init_lib() )
        FATAL_ERROR_MSG("Library not initialized correctly\n");

	/** Create shared data structure */
	data = create_data(count);

	data->parent_thread = GetCurrentThread();

    data->ready_event = CreateEvent(NULL, TRUE, FALSE, TEXT("ReadyEvent"));

    if ( !data->ready_event )
        FATAL_ERROR_MSG("CreateEvent failed: %s\n", GetLastError());

    data->semaphore = CreateSemaphore(NULL, count, count, NULL);

	if ( !data->semaphore )
		FATAL_ERROR_MSG("Semaphore creation failed: %s\n", strerror(errno));

	ret = lib.init(count, speed, seed);

	if ( ret == -1 )
		FATAL_ERROR_MSG("Library initialization failed\n");

	for ( i = 0; i < count; ++i ) {
		data->children[i].status = PID_STATUS_DEAD;
        // We could allocate a char here, and deallocate it in the thread,
        // but pointer arithmetic is cool
		current_pid = CreateThread(NULL, 0, child_proc, data->children + i, 0);
		switch ( current_pid ) {
			case -1:
				perror("fork");
				__dump();
				ERROR_EXIT();
			case 0:
				/** We must ensure current_index is ok in child_proc */
				data->children[i].id = GetCurrentThreadId();
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
