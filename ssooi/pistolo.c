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
#include <sys/shm.h>
#include <string.h>
#include <time.h>
#define DEFAULT_PROC_COUNT 7
// #define DEBUG

typedef struct GameData {
	size_t process_count;
	sig_atomic_t die_allowed;
	pid_t parent_id;
	pid_t *pids;
} GameData;

typedef enum DataActionType {
	DATA_CREATE,
	DATA_GET,
	DATA_RELEASE
} DataActionType;
/** Wrapper macros for GameData actions */
#define get_data() manage_data(DATA_GET, 0)
#define release_data() manage_data(DATA_RELEASE, 0)
#define create_data(process_count) manage_data(DATA_CREATE, process_count)

/** Get game data, create if non null passed */
GameData * manage_data(DataActionType, size_t);

/** Get a shared block of memory */
void * get_shared_mem(size_t, int *);

/** Display pids alive */
void show_pids();

/** Kill all pending processes */
void kill_all();

/** Catch SIGUSR1 signal (shoot to a brother) */
void child_sigusr_catch(int);

/** Catch SIGTERM signal (die, probably) */
void child_sigterm_catch(int);

/** Bind signals to parent proc so children */
int bind_children_signals_and_lock();

/** Get a random brother PID */
pid_t child_rand_pid();

/** Child process logic, mostly setting signal handlers and wait for them */
int child_proc();

/** Parent process logic */
int parent_proc();

/** Get a shared block of memory */
void * get_shared_mem( size_t size, int *id ) {
	int shmid = shmget(time(NULL), size, IPC_CREAT | 0777);
	void *ret = shmat(shmid, NULL, 0);

	if ( ret == (void *) -1 )
		perror("Failed getting memory");

	if ( id != NULL )
		*id = shmid;

	return ret;
}


/** Manage game data */
GameData * manage_data(DataActionType action, size_t count) {
	static GameData *data = NULL;
	static int memid;

	if ( action == DATA_CREATE && count != 0 && data == NULL ) {
		data = (GameData *) get_shared_mem(sizeof(GameData) + sizeof(pid_t) * count, &memid);
		data->process_count = count;
		data->pids =  (pid_t *) (data + 1);
		data->die_allowed = 0;
		data->parent_id = 0;
	}

	if ( action == DATA_RELEASE ) {
		shmctl(memid, IPC_RMID, NULL);
		data = NULL;
	}

	return data;
}

/** Display pids */
void show_pids() {
	size_t i = 0;
	GameData *data = get_data();
	pid_t *pids = data->pids;
	size_t count = data->process_count;

	for ( ; i < count; i++ ) {
		if ( pids[i] != 0 ) {
			printf("%d\t", pids[i]);
		}
	}
	printf("\n");
}

/** Kill all pending processes */
void kill_all() {
	GameData *data = get_data();
	pid_t *pids = data->pids;
	size_t count = data->process_count;

	while ( count-- ) {
		if ( pids[count] ) {
			kill(pids[count], SIGKILL);
		}
	}
}

/** Get a pid and shoot */
void child_sigusr_catch( int sig ) {
	pid_t random_pid,
		current_pid = getpid();
	GameData *data = get_data();

#ifdef DEBUG
	pid_t wait_response;
#endif

	random_pid = child_rand_pid();

	if ( random_pid == -1 )
		return; // no shootable

	// If processes can't die, we won't shoot
	if ( ! data->die_allowed )
		return;

	printf("%d->%d\n", current_pid, random_pid);
	kill(random_pid, SIGTERM);
}

void child_sigterm_catch( int sig ) {
	pid_t current_pid = getpid();
	size_t i = 0;
	GameData *data = get_data();
	pid_t *pids = data->pids;
	size_t count = data->process_count;

#ifdef DEBUG
	printf("(%d) catched SIGTERM (exiting: %d)\n", getpid(), data->die_allowed);
#endif

	if ( ! data->die_allowed ) {
#ifdef DEBUG
		printf("%d survives\n", getpid());
#endif
		return;
	}

	for ( ; i < count; i++ ) {
		if ( pids[i] == current_pid ) {
			pids[i] = 0;
			break;
		}
	}

	exit(0);
}


/** Bind signals to parent proc so children */
int bind_children_signals_and_lock() {
	sigset_t set;

	// Block SIGTERM and SIGUSR1
	sigemptyset(&set);
	sigaddset(&set, SIGTERM);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);

	// Add handler for SIGUSR1
	if ( signal(SIGUSR1, child_sigusr_catch) == SIG_IGN ) {
		printf("Handler not registered\n");
		return -1;
	}

	// Add handler for SIGTERM
	if ( signal(SIGTERM, child_sigterm_catch) == SIG_IGN ) {
		printf("Handler not registered\n");
		return -1;
	}

	return 0;
}

/** Get a random pid */
pid_t child_rand_pid() {
	pid_t current_pid = getpid();

	size_t i = 0,
		shootable_count = 0,
		random,
		count;

	GameData *data = get_data();
	pid_t *pids = data->pids;

	count = data->process_count;

	// Loop over PIDS and get those which are not self or 0
	// Get a random between them an iterate again...
	// I know this is far from optimal
	// The other way was the function commented above
	for ( ; i < count; i++ ) {
		if ( pids[i] != 0 && pids[i] != current_pid ) {
			shootable_count++;
		}
	}

	if ( shootable_count == 0 ) {
		return -1;
	}

	random = rand() % shootable_count;

	for ( i = 0;  i < count; i++ ) {
		if ( pids[i] != 0 && pids[i] != current_pid ) {
			if ( random == 0 ) {
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
	// GameData *data = get_data();

#ifdef DEBUG
	printf("Child started\n");
#endif

	// Set the PRNG with getpid() to get different values for different procs
	srand(time(NULL) + getpid());

	// Only allow to catch SIGUSR1 and SIGTERM
	sigfillset(&set);
	sigdelset(&set, SIGUSR1);
	sigdelset(&set, SIGTERM);

	while( 1 ) {
		sigsuspend(&set);
	}

	return 0;
}


/**
 * Parent process subroutine
 * @see get_data()
 */
int parent_proc() {
	size_t rounds = 1,
		i,
		already_dead,
		total_dead = 0;
	GameData *data = get_data();
	size_t count = data->process_count;
	pid_t *pids = data->pids;

	// Game loop
	while ( 1 ) {
		already_dead = total_dead;
		total_dead = 0;

		// check for deads in the last round
		for ( i = 0; i < count; i++) {
			if ( pids[i] == 0 ) {
				total_dead++;
			}
		}

		if ( total_dead == count || total_dead == count - 1 )
			break;

		printf("\n----------------------\n");
		printf("Round #%zu\n", rounds);
		printf("Total: %zu\n", ( size_t ) count);
		printf("Dead: %zu\n", total_dead);
		printf("Last round: %zu\n", total_dead - already_dead);
		printf("Alive (%zu):\n", count - total_dead);
		show_pids();
		printf("----------------------\n");

		// Let them shoot
		data->die_allowed = 1;
		for ( i = 0; i < count; i++ ) {
			if ( pids[i] != 0 && kill(pids[i], SIGUSR1) != 0 ) {
				if ( errno == ESRCH ) {
					printf("PIDS[%zu] dead (%d)\n", i, pids[i]);
				} else {
					perror("failed at kill");
				}
			}
		}

		++rounds;
		sleep(1);
		data->die_allowed = 0;
	}

	printf("Game is over!\n");
	if ( total_dead == count ) {
		printf("No shooter is alive\n");
	} else {
		// Only should be one
		printf("%zu shooters alive\n", count - total_dead);
		show_pids();
	}

	// kill all pending procs
	kill_all();

	return 0;
}

int main(int argc, char **argv) {
	size_t count = DEFAULT_PROC_COUNT;
	pid_t current_pid;
	int ret;
	GameData *data;

	if ( argc > 1 ) {
		count = strtoul(argv[1], NULL, 10);
	}

	/** Create shared data structure */
	data = create_data(count);

	data->parent_id = getpid();

	/** This way all children have signals registered */
	bind_children_signals_and_lock();

	/** Process creation */
	while ( count-- ) {
		current_pid = fork();
		if ( current_pid == 0 ) {
			return child_proc();
		}

		data->pids[count] = current_pid;
	}

	ret = parent_proc();

	release_data();

	return ret;
}
