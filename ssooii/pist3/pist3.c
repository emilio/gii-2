#define _UNICODE
#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* strlen, strerror */
#include <time.h> /* time */
#include <windows.h>
#define ERROR_EXIT() exit(100)

// We're forced to compile with Dev-CPP :/
#ifdef __cplusplus
extern "C" {
#endif

#define ERROR_MSG(str, ...) do { \
    fprintf(stderr, str "\n", ##__VA_ARGS__); \
    LOG(str, ##__VA_ARGS__); \
} while (0)

#define FATAL_ERROR_MSG(str, ...) do { \
    ERROR_MSG(str, ##__VA_ARGS__); \
    ERROR_EXIT(); \
} while( 0 )

#ifdef DEBUG
#define LOG_PATH "pist3.log"
#define LOG(msg, ...) do { \
    GameData* data = get_data(); \
    if ( data && WaitForSingleObject(data->log_mutex, INFINITE) == WAIT_OBJECT_0 ) { \
        if ( fseek(data->log, 0, SEEK_END) > -1 ) { \
            fprintf(data->log, "(%d) "msg"\n", GetCurrentThreadId(), ##__VA_ARGS__); \
        } \
        fflush(data->log); \
        ReleaseMutex(data->log_mutex); \
    } else { \
        fprintf(stderr, "Log error: %s", GetLastError()); \
    } \
} while (0)
#else
#define LOG(...)
#endif

#define LIMIT_MIN 3
#define LIMIT_MAX 26
#define MAX_CHILDREN LIMIT_MAX

typedef enum DataActionType {
    DATA_CREATE,
    DATA_REFRESH,
    DATA_GET,
    DATA_RELEASE
} DataActionType;

#define PID_STATUS_READY 0x01
#define PID_STATUS_SHOT 0x02
#define PID_STATUS_DEAD_THIS_ROUND 0x04
#define PID_STATUS_DEAD 0x08

// We need three semaphores:
//    - We'll use one for telling the parent or coordinator we're
//        ready to go.
//    - We need other too wait for pass and avoid conflits that
//        can happen if we reuse the same semaphore in both
//        parts of the round
#define SEMAPHORE_COUNT 3

typedef struct GameData {
    /* const */ size_t process_count; /** Number of processes playing. This is inmutable */
    size_t rounds; /** The round count */
    size_t last_thread;
    long alive_count; /** Current round alive pids count */
    HANDLE threads[MAX_CHILDREN];
    int statuses[MAX_CHILDREN];
    HANDLE semaphores[SEMAPHORE_COUNT];
#ifdef DEBUG
    HANDLE log_mutex;
    FILE* log;
#endif
} GameData;

GameData * manage_data(DataActionType, size_t);

#define get_data() manage_data(DATA_GET, 0)
#define refresh_data() manage_data(DATA_REFRESH, 0)
#define release_data() manage_data(DATA_RELEASE, 0)
#define create_data(process_count) manage_data(DATA_CREATE, process_count)

// Library management
struct lib {
       int (*init)(int, int, int);
       int (*new_shooter)(char);
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
    lib.init = (int (*)(int, int, int)) GetProcAddress(dlllib, "PIST_inicio");
    lib.new_shooter = (int (*) (char)) GetProcAddress(dlllib, "PIST_nuevoPistolero");
    lib.victim = (char (*) (void)) GetProcAddress(dlllib, "PIST_vIctima");
    lib.shoot = (int(*)(char)) GetProcAddress(dlllib, "PIST_disparar");
    lib.die = (void (*) (void)) GetProcAddress(dlllib, "PIST_morirme");
    lib.deinit = (int (*) (void)) GetProcAddress(dlllib, "PIST_fin");
    lib.error = (void (*)(char*)) GetProcAddress(dlllib, "pon_error");

    if ( lib.init == NULL ||
         lib.new_shooter == NULL ||
         lib.victim == NULL ||
         lib.shoot == NULL ||
         lib.die == NULL ||
         lib.deinit == NULL ||
         lib.error == NULL )
        return false;

    return true;
}

/** Debugging and resource management */
void kill_all();
void release_all_data();

/** Utilities */
void program_help();

/** Game logic */
bool is_current_proc_coordinator(size_t);
DWORD child_proc(void*);

// Print program help
void program_help() {
    fprintf(stderr, "Usage: ./pist3 <count> <speed> <seed>\n");
    ERROR_EXIT();
}

// Check if the current process is the coordinator
// We do an index check
bool is_current_proc_coordinator(size_t current_index) {
    GameData* data = get_data();
    size_t i;

    i = data->process_count;
    while ( i-- ) {
        if ( ~data->statuses[i] & PID_STATUS_DEAD ) {
            if ( i == current_index )
                return 1;
            else
                return 0;
        }
    }

    return 0;
}

GameData* manage_data(DataActionType action, size_t count) {
    static GameData* data = NULL;

    if ( action == DATA_CREATE && count != 0 && data == NULL ) {
        data = (GameData*) malloc(sizeof(GameData));
        data->process_count = count;
        data->alive_count = count;
        data->rounds = 0;
        data->last_thread = 0;
        for ( size_t i = 0; i < SEMAPHORE_COUNT; ++i )
            data->semaphores[i] = NULL;
#ifdef DEBUG
        data->log = fopen(LOG_PATH, "w+");
        if ( !data->log )
            data->log = stderr;
#endif
    }

    if ( action == DATA_RELEASE ) {
        for ( size_t i = 0; i < SEMAPHORE_COUNT; ++i )
            if ( data->semaphores[i] )
                if ( !CloseHandle(data->semaphores[i]) )
                    LOG("Release semaphore failed: %s\n", GetLastError());

#ifdef DEBUG
        if ( data->log_mutex )
            if ( !CloseHandle(data->log_mutex) )
                LOG("Release mutex failed: %s\n", GetLastError());

        if ( data->log )
            fclose(data->log);
#endif

        free( (void *) data);
        data = NULL;
    }

    return data;
}

void kill_all() {
    GameData *data = get_data();

    for (size_t i = 0; i < data->process_count; ++i )
        TerminateThread(data->threads[i], 1);
}

void release_all_resources() {
    int ret;

    ret = lib.deinit();
    if ( ret == -1 )
        LOG("Library termination failed\n");

    kill_all();
    release_data();
    fflush(stderr);
    fclose(stderr);
}

DWORD child_proc(void* my_status_ptr_) {
    GameData* data = get_data();
    size_t my_index = ((int*)my_status_ptr_) - (int*)data->statuses;
    char lib_id = 'A' + my_index;
    char target;
    int ret;
    bool im_coordinator;
    size_t this_round_alive_count = 0;

    data->statuses[my_index] = PID_STATUS_READY;

    LOG("Thread %d started, index: %d", GetCurrentThreadId(), my_index);

    ret = lib.new_shooter(lib_id);
    if ( ret == -1 )
        FATAL_ERROR_MSG("Shooter not initialized");

    // We tell the parent we're ready
    // And wait for it to reply back when all the threads are awake
    ReleaseSemaphore(data->semaphores[0], 1, NULL);
    WaitForSingleObject(data->semaphores[2], INFINITE);

    LOG("Ready to roll");

    while ( 1 ) {
        im_coordinator = is_current_proc_coordinator(my_index);

        if ( im_coordinator ) {
            this_round_alive_count = data->alive_count;
            if ( data->alive_count == 1 ) {
                data->last_thread = GetCurrentThreadId();
                return 0;
            }
            data->rounds++;
            LOG("Round: %d, alive: %d", data->rounds, data->alive_count);
        }
        LOG("Starting to shoot");

        target = lib.victim();
        if ( target == '@' )
            FATAL_ERROR_MSG("This shit blew up\n");

        // Mark the target as shot, equivalent to send the "DEAD" message
        data->statuses[target - 'A'] |= PID_STATUS_DEAD_THIS_ROUND;

        if ( lib.shoot(target) == -1 )
            ERROR_MSG("PIST_disparar");

        LOG("Shot to %c, waiting for second part", target);

        // The same idea
        ReleaseSemaphore(data->semaphores[0], 1, NULL);

        if ( im_coordinator ) {
            size_t i = this_round_alive_count;
            LOG("Waiting for everyone to be ready");
            while ( i-- ) {
                WaitForSingleObject(data->semaphores[0], INFINITE);
                LOG("Reveived signal, %d left", i);
            }
            ReleaseSemaphore(data->semaphores[1], this_round_alive_count, NULL);
        }
        WaitForSingleObject(data->semaphores[1], INFINITE);

        // If we have received a shot, mark as dead, else... we're ready
        // for the next round
        if ( data->statuses[my_index] & PID_STATUS_DEAD_THIS_ROUND ) {
            LOG("Reveived a shot");
            data->statuses[my_index] |= PID_STATUS_DEAD;
            InterlockedDecrement(&data->alive_count);
            lib.die();
        } else {
            LOG("They missed");
            data->statuses[my_index] = PID_STATUS_READY;
        }

        // The same strategy here
        ReleaseSemaphore(data->semaphores[0], 1, NULL);
        if ( im_coordinator ) {
            size_t i = this_round_alive_count;
            while ( i-- ) {
                LOG("Waiting for everyone to be ready, pending: %d", i);
                WaitForSingleObject(data->semaphores[0], INFINITE);
            }
            LOG("All ready");
            ReleaseSemaphore(data->semaphores[2], this_round_alive_count, NULL);
        }
        WaitForSingleObject(data->semaphores[2], INFINITE);

        // Die if we do not count anymore
        if ( data->statuses[my_index] & PID_STATUS_DEAD )
            return 0;
    }

    return 0;
}

int main(int argc, char **argv) {
    size_t i = 0,
        count = 0,
        speed = 0,
        seed = 0;
    GameData *data;
    int ret;

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

    if ( count < LIMIT_MIN || count > LIMIT_MAX )
        FATAL_ERROR_MSG("Number of processes must be between %d and %d.\n", LIMIT_MIN, LIMIT_MAX);

    if ( count == 0 )
        FATAL_ERROR_MSG("At least one player is required.\n");

    atexit(release_all_resources);

    if ( ! init_lib() )
        FATAL_ERROR_MSG("Library not initialized correctly\n");


    ret = lib.init(count, speed, seed);
    if ( ret == -1 )
        FATAL_ERROR_MSG("Library initialization failed\n");

    data = create_data(count);

    for ( size_t i = 0; i < SEMAPHORE_COUNT; ++i )
        if ( !(data->semaphores[i] = CreateSemaphore(NULL, 0, count, NULL)) )
            FATAL_ERROR_MSG("Semaphore creation failed: %s\n", GetLastError());

#ifdef DEBUG
    data->log_mutex = CreateMutex(NULL, FALSE, TEXT("LogMutex"));
    if ( !data->log_mutex )
       FATAL_ERROR_MSG("Log mutex initialization failed: %s\n", GetLastError());
#endif

    for ( i = 0; i < count; ++i ) {
        data->statuses[i] = PID_STATUS_DEAD;

        // We could allocate a char here, and deallocate it in the thread,
        // but pointer arithmetic is cool
        data->threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) child_proc, data->statuses + i, 0, NULL);
        if ( ! data->threads[i] )
           FATAL_ERROR_MSG("CreateThread");
     }

    size_t count_ = count;

    // We wait for all the processes to tell us they're ready
    while ( count_-- ) {
        LOG("Parent waiting for children, pending: %d", count_);
        WaitForSingleObject(data->semaphores[0], INFINITE);
    }
    LOG("All should be ready now");
    ReleaseSemaphore(data->semaphores[2], count, NULL);

    LOG("Waiting for all threads to exit");

    // Now they take care of the job, we must  wait for them to die
    WaitForMultipleObjects(count, data->threads, TRUE, INFINITE);

    return data->last_thread;
}

#ifdef __cplusplus
} // Extern "C"
#endif

