#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "semaphore/semaphore.h"

void noop(int sig) {}

void critical_section() {
	static int count = 0;

	printf("<%d> critical section start (%d)\n", getpid(), ++count);
	// sleep(5);
	printf("<%d> critical section end (%d)\n", getpid(), count);
}

int main() {
	int i = 100;
	semaphore_t* sem = get_shared_semaphore("__sem_temp");

	signal(SIGUSR1, noop);

	semaphore_init(sem, 1);

	fork();

	while ( i-- ) {
		semaphore_lock(sem);
		critical_section();
		semaphore_unlock(sem);
	}

	release_shared_semaphore(sem, "__sem_temp");

	return 0;
}
