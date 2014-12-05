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

/**
 * The GameData struct is the main data wrapper for our game
 *
 * With the function manage_data() and three helper macros we can manage it
 */

/** The  action type passed to manage_data */
typedef enum DataActionType {
	DATA_CREATE,
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
	PID_STATUS_DEAD = 0x04
} PidStatusType;

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
	sig_atomic_t not_ready_yet; /** Number of processes not ready yet.
									It starts being the process_count and decrements to zero
									while child processes are loaded */
	pid_t *pids; /** The pid array, dinamically allocated */
	PidStatusType *pid_status; /** The pid status array, dinamically allocated too.
									If pid is alive it gets updated to PID_STATUS_READY
									at the beginning of each round */
} GameData;

/** Get the main game data struct, create if non-zero value is passed as a second argument */
GameData * manage_data(DataActionType, size_t);

/** Wrapper macros for GameData actions */
#define get_data() manage_data(DATA_GET, 0)
#define release_data() manage_data(DATA_RELEASE, 0)
#define create_data(process_count) manage_data(DATA_CREATE, process_count)

/** Check if current round is over */
int round_over();

/**
 * Get a shared block of memory
 * The first argument is the size
 * The second one is a reference where the id of the block will be stored
 */
void * get_shared_mem(size_t, int *);

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

/** Catch sigusr signal, empty more than probably */
void parent_sigusr_catch(int);

/** Bind signals to parent proc so they will be bound to children */
void bind_children_signals_and_lock();

/** Bind signals to parent proc only*/
void bind_parent_signals();

/** Get a random brother PID */
pid_t child_rand_pid();

/** Get current child index */
size_t current_index();

/** Child process logic, mostly wait for signals */
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
	static int memid;

	if ( action == DATA_CREATE && count != 0 && data == NULL ) {
		data = (GameData *) get_shared_mem(sizeof(GameData) + sizeof(pid_t) * count + sizeof(PidStatusType) * count, &memid);
		data->process_count = count;
		data->parent_id = 0;
		data->rounds = 0;
		data->not_ready_yet = count;
		data->pids =  (pid_t *) (data + 1);
		data->pid_status = (PidStatusType *) (data->pids + count);
	}

	if ( action == DATA_RELEASE ) {
		shmctl(memid, IPC_RMID, NULL);
		data = NULL;
	}

	return data;
}

/** Print an integer list */
void print(int *list, size_t length) {
	size_t i = 0;

	for ( ; i < length; i++ )
		printf("%d\t", list[i]);

	printf("\n");
}

/** Show list of procs */
void show_pids() {
	GameData *data = get_data();
	pid_t *pids = data->pids;
	size_t i = 0,
		count = data->process_count;

	for ( ; i < count; ++i )
		if ( pids[i] )
			printf("%d\t", pids[i]);

	printf("\n");
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
	GameData *data = get_data();

#ifdef DEBUG
	printf("Child SIGUSR catch\n");
#endif

	random_pid = child_rand_pid();


	if ( random_pid == -1 || // no available pid
		random_pid == 0 || // shot by another process
		data->pid_status[current_index()] & PID_STATUS_DEAD ) // or we are already dead
		return;

	printf("%d->%d\n", current_pid, random_pid);
	data->pid_status[current_index()] |= PID_STATUS_SHOT;
	kill(random_pid, SIGTERM);
}

void child_sigterm_catch( int sig ) {
	GameData *data = get_data();


#ifdef DEBUG
	printf("(%d) catched SIGTERM\n", getpid());
	printf("Status: %d\n", data->pid_status[current_index()]);
#endif

	data->pid_status[current_index()] |= PID_STATUS_DEAD;
	data->pids[current_index()] = 0;

	// Tell parent we're done
	kill(data->parent_id, SIGUSR2);

	exit(0);
}

void parent_sigusr_catch( int sig ) {
#ifdef DEBUG
	printf("Parent received %s\n", sig == SIGUSR2 ? "SIGUSR2" : "SIGALRM");
#endif
}

/** Bind signals to parent proc so children */
void bind_children_signals_and_lock() {
	sigset_t set;

	// Block SIGTERM and SIGUSR1
	sigemptyset(&set);
	sigaddset(&set, SIGTERM);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);

	// Add handler for SIGUSR1
	signal(SIGUSR1, child_sigusr_catch);
	signal(SIGTERM, child_sigterm_catch);

}

void bind_parent_signals() {
	sigset_t set;

	// Unlock SIGTERM for parent
	sigemptyset(&set);
	sigaddset(&set, SIGTERM);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_UNBLOCK, &set, NULL);

	signal(SIGUSR1, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGUSR2, parent_sigusr_catch);
	signal(SIGALRM, parent_sigusr_catch); // Since it's just a fallback...

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
	for ( ; i < count; i++ )
		if ( pids[i] != 0 && pids[i] != current_pid )
			shootable_count++;

	if ( shootable_count == 0 )
		return -1;

	random = rand() % shootable_count;

	for ( i = 0;  i < count; i++ ) {
		if ( pids[i] && pids[i] != current_pid ) {
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
	GameData *data = get_data();

	// Set the PRNG with getpid() to get different values for different procs
	srand(time(NULL) + getpid());


	// Only allow to catch SIGUSR1, SIGUSR2 and SIGTERM
	sigfillset(&set);
	sigdelset(&set, SIGUSR1);
	sigdelset(&set, SIGTERM);

#ifdef DEBUG
	printf("Child started\n");
#endif

	// When all ready, call parent
	// if ( --data->not_ready_yet == 0 ) // But sync
	if ( __sync_sub_and_fetch(&data->not_ready_yet, 1) == 0 )
		kill(data->parent_id, SIGUSR2);

	while ( 1 )
		sigsuspend(&set);

	return 0;
}

/**
 * Check if current round is over
 * Current round is over if none of pid_status is PID_STATUS_SHOT
 */
int round_over() {
	GameData *data = get_data();
	PidStatusType *pid_status = data->pid_status;
	size_t count = data->process_count,
		i = 0;

	// If someone is still ready the round isn't still over
	for ( ; i < count; ++i )
		if ( pid_status[i] == PID_STATUS_READY )
			return 0;

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
	sigset_t set;

	// Parent waits for sigusr2
	sigfillset(&set);
	sigdelset(&set, SIGUSR2);
	sigdelset(&set, SIGALRM);
	// For debugging and stopping the program
	sigdelset(&set, SIGTERM);

	// Wait for ready, three seconds if fail
	alarm(3);
	sigsuspend(&set);
	alarm(0);

	printf("All ready to start\n");

	// Game loop
	while ( 1 ) {
		already_dead = total_dead;
		total_dead = 0;

		for ( i = 0; i < count; ++i ) {
			// update dead pids (done in the sigterm catch, but we ensure sync here)
			if ( pids[i] && waitpid(pids[i], NULL, WNOHANG) == pids[i] )
				pids[i] = 0;

			// check for deads in the last round
			if ( pids[i] == 0 )
				++total_dead;
			else // Set them as ready
				pid_status[i] = PID_STATUS_READY;
		}

		if ( total_dead == count || total_dead == count - 1 )
			break;

		++data->rounds;

		printf("\n----------------------\n");
		printf("Round #%zu\n", data->rounds);
		printf("Total: %zu\n", ( size_t ) count);
		printf("Dead: %zu\n", total_dead);
		printf("Last round: %zu\n", total_dead - already_dead);
		printf("Alive (%zu):\n", count - total_dead);
		show_pids();
		printf("----------------------\n");

		// Let them shoot
		for ( i = 0; i < count; ++i )
			if ( pids[i] )
				kill(pids[i], SIGUSR1);

		do {
			alarm(1); // Needed because signal syncing sucks
			sigsuspend(&set);
		} while( ! ( round_over() ) );


		alarm(0); // cancel alarms
	}

	// kill all pending procs
	kill_all();

	printf("Game is over!\n");

	if ( total_dead == count )
		printf("No shooter is alive\n");
	else
		printf("%zu shooters alive\n", count - total_dead); // Only should be one

	show_pids();

	return 0;
}

/** Main process logic */
int main(int argc, char **argv) {
	size_t count = DEFAULT_PROC_COUNT;
	pid_t current_pid;
	int ret;
	GameData *data;

	if ( argc > 1 )
		count = strtoul(argv[1], NULL, 10);

	/** Create shared data structure */
	data = create_data(count);

	data->parent_id = getpid();

	/** This way all children have signals registered */
	bind_children_signals_and_lock();

	/** Process creation */
	while ( count-- ) {
		current_pid = fork();

		if ( current_pid == 0 )
			return child_proc();

		data->pids[count] = current_pid;
	}

	bind_parent_signals();

	ret = parent_proc();

	release_data();

	return ret;
}
