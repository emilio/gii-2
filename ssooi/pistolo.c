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
#define PROC_COUNT 25
// #define DEBUG 1

pid_t *PIDS;

volatile sig_atomic_t sigterm_exit = 1;

void show_pids();
void child_sigusr_catch(int);
void child_sigterm_catch(int);
pid_t child_rand_pid();
int child_proc();
void *get_shared_mem(size_t);
int parent_proc();

/** Display pids */
void show_pids() {
	size_t i = 0;
	for ( ; i < PROC_COUNT; i++ ) {
		if ( PIDS[i] != 0 ) {
			printf("%d\t", PIDS[i]);
		}
	}
	printf("\n");
}

/** Get a pid and shoot */
void child_sigusr_catch( int sig ) {
	pid_t random_pid,
		current_pid = getpid();

#ifdef DEBUG
	pid_t wait_response;
#endif
	int random_pid_status;


	random_pid = child_rand_pid();

	if ( random_pid == -1 ) {
		return; // no shootable
	}

	printf("%d->%d\n", current_pid, random_pid);

	// Don't allow sigterm exit while shooting
	sigterm_exit = 0;
	kill(random_pid, SIGTERM);

	// Syncronous wait to random pid
#ifndef DEBUG
	waitpid(random_pid, &random_pid_status, 0);
#else
	wait_response = waitpid(random_pid, &random_pid_status, 0);
	printf("(%d) %d status: %d response: %d\n", current_pid, random_pid, random_pid_status, wait_response);
	if ( WIFEXITED(random_pid_status) ) {
		printf("Exited: %d\n", WEXITSTATUS(random_pid_status));
		if ( WEXITSTATUS(random_pid_status) == 1 ) {
			printf("%d dead\n", random_pid);
		}
	}
#endif

	sigterm_exit = 1;
}

void child_sigterm_catch( int sig ) {
	pid_t current_pid = getpid();
	size_t i = 0;
#ifdef DEBUG
	printf("(%d) catched SIGTERM (exiting: %d)\n", getpid(), sigterm_exit);
#endif
	if ( ! sigterm_exit ) {
		printf("%d survives\n", getpid());
		return;
	}

	// Consider remove this assignment
	for ( ; i < PROC_COUNT; i++ ) {
		if ( PIDS[i] == current_pid ) {
			PIDS[i] = 0;
			break;
		}
	}
	exit(1); // If killed exit 1
}

/** Get a random pid, faster, but doesn't always shoot */
// pid_t child_rand_pid() {
// 	pid_t current_pid = getpid(),
// 		random_pid;

// 	random_pid = PIDS[rand() % PROC_COUNT];

// 	if ( random_pid == 0 || random_pid == current_pid ) {
// 		return -1;
// 	}

// 	return random_pid;
// }

/** Get a random pid */
pid_t child_rand_pid() {
	pid_t current_pid = getpid();

	size_t i = 0,
		shootable_count = 0,
		random;

	// Loop over PIDS and get those which are not self or 0
	// Get a random between them an iterate again...
	// I know this is far from optimal
	// The other way was the function commented above
	for ( ; i < PROC_COUNT; i++ ) {
		if ( PIDS[i] != 0 && PIDS[i] != current_pid ) {
			shootable_count++;
		}
	}

	if ( shootable_count == 0 ) {
		return -1;
	}

	random = rand() % shootable_count;

	for ( i = 0;  i < PROC_COUNT; i++ ) {
		if ( PIDS[i] != 0 && PIDS[i] != current_pid ) {
			if ( random == 0 ) {
				return PIDS[i];
			}
			random--;
		}
	}

	return -1;
}

/** Child process subroutine */
/** Note self is an index, not a pid */
int child_proc(pid_t parent_id) {
	sigset_t set;

#ifdef DEBUG
	printf("Child started\n");
#endif

	// Set the PRNG with getpid() to get different values for different procs
	srand(time(NULL) + getpid());

	// Add handler for SIGUSR1
	if ( signal(SIGUSR1, child_sigusr_catch) == SIG_IGN ) {
		printf("Handler not registered\n");
		return 1;
	}

	// Add handler for SIGTERM
	if ( signal(SIGTERM, child_sigterm_catch) == SIG_IGN ) {
		printf("Handler not registered\n");
		return 1;
	}

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
 * @see PIDS
 * @see PROC_COUNT
 */
int parent_proc() {
	size_t rounds = 1,
		i,
		already_dead,
		total_dead = 0;
	// int *statuses = (int *) calloc(PROC_COUNT, sizeof(int));

	sleep(2);

	// Game loop
	while ( 1 ) {
		already_dead = total_dead;
		total_dead = 0;

		// check for deads in the last round
		for ( i = 0; i < PROC_COUNT; i++) {
			// If wasn't dead and now it is...
			// if ( PIDS[i] != 0 && waitpid(PIDS[i], &statuses[i], WNOHANG) == PIDS[i] ) {
			// 	// dead
			// 	PIDS[i] = 0;
			// 	dead++;
			// }

			if ( PIDS[i] == 0 ) {
				total_dead++;
			}
		}

		if ( total_dead == PROC_COUNT || total_dead == PROC_COUNT - 1 )
			break;

		printf("----------------------\n");
		printf("Round #%zu\n", rounds);
		printf("Total dead: %zu\nLast round: %zu\n", total_dead, total_dead - already_dead);
		show_pids();
		printf("----------------------\n");

		// Let them shoot
		for ( i = 0; i < PROC_COUNT; i++ ) {
			if ( PIDS[i] != 0 && kill(PIDS[i], SIGUSR1) != 0 ) {
				if ( errno == ESRCH ) {
					printf("PIDS[%zu] dead (%d)\n", i, PIDS[i]);
				} else {
					printf("%s\n", strerror(errno));
				}
			}
		}

		rounds++;
		// show_statuses(statuses, count);
		sleep(2);
	}

	printf("Game is over!\n");
	if ( total_dead == PROC_COUNT ) {
		printf("No shooter is alive\n");
		return 1;
	}

	// Only should be one
	printf("%zu shooters alive\n", PROC_COUNT - total_dead);
	show_pids();

	return 0;
}

void *get_shared_mem(size_t size) {
	key_t key = time(NULL);
	int id = shmget(key, size, IPC_CREAT | 0777);
	void *ret = shmat(id, NULL, 0);

	if ( ret == (void *) -1 )
		printf("%s\n", strerror(errno));



	return ret;
}

int main() {
	size_t i = PROC_COUNT;
	pid_t parent_id = getpid();
	pid_t current_pid;
	PIDS = (pid_t *) get_shared_mem(sizeof(pid_t) * PROC_COUNT);

	/** Process creation */
	while ( i-- ) {
		current_pid = fork();
		if ( current_pid == 0 ) {
		// if ( ! ( PIDS[i] = fork() ) ) {
			return child_proc(parent_id);
		}

		PIDS[i] = current_pid;
	}

	parent_id = 0;

	return parent_proc();
}
