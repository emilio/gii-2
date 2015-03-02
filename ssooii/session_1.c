
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore/semaphore.h>
#include <macros/ensure_success.h>

#define MAX_PROCS 10
#define MAX_REPETITIONS 10
void critical_section() {
	printf("<%d> critical...", getpid());
	fflush(stdout);
	sleep(1);
	printf(" end\n");
}

int main(int argc, char** argv) {
	size_t i = 0;
	/** Our semaphore key will use pid as id */
	pid_t parent_id = getpid();
	key_t key = ftok(argv[0], parent_id);
	semaphore_t sem = semaphore_get(key);

	ENSURE_SUCCESS(sem);
	ENSURE_SUCCESS(semaphore_set_value(sem, 1));

	/** Create the children */
	for ( i = 0; i < MAX_PROCS; ++i )
		if ( fork() == 0 )
			break;

	for ( i = 0; i < MAX_REPETITIONS; ++i ) {
		ENSURE_SUCCESS(semaphore_lock(sem));
		critical_section();
		ENSURE_SUCCESS(semaphore_unlock(sem));
	}

	if ( parent_id == getpid() ) {
		semaphore_wait(sem);
		semaphore_destroy(sem);
	}



	return 0;
}
