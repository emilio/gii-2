#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "semaphore/semaphore.h"

#define PROC_COUNT 10
#define ITERATIONS 10
#define SEMAPHORE_SHARED_FILE "__sem.tmp"

void noop(int sig) {}

void critical_section() {
    static int count = 0;

    printf("<%d> critical section start (%d)\n", getpid(), ++count);
    sleep(rand() % 5);
    printf("<%d> critical section end (%d)\n", getpid(), count);
}

int main() {
    int i;
    pid_t parent = getpid(), dead_child;

    semaphore_t* sem = get_shared_semaphore(SEMAPHORE_SHARED_FILE);

    signal(SEMAPHORE_RESUME_SIGNAL_NUMBER, noop);

    semaphore_init(sem, 5);

    for (i = 0; i < PROC_COUNT; ++i)
        if (fork() == 0)
            break;

    srand(time(NULL) + getpid() + getpid() % PROC_COUNT);

    for (i = 0; i < ITERATIONS; ++i) {
        semaphore_lock(sem);
        critical_section();
        semaphore_unlock(sem);
    }

    if (parent != getpid()) {
        printf("%d ended\n", getpid());
        return 0;
    }

    printf("Parent got to the end\n");

    i = PROC_COUNT;
    while (i) {
        printf("Waiting... \n");
        dead_child = wait(NULL);
        printf("... got %d\n", dead_child);
        i--;
    }
    release_shared_semaphore(sem, SEMAPHORE_SHARED_FILE);

    return 0;
}
