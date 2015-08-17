#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore/semaphore.h>
#include <macros/ensure_success.h>

#define NUM_PROCS 3

semaphore_t sem;

int aem();
void cleanup(int);

void critical_section();

int main(int argc, char** argv) {
    size_t i;
    key_t key = ftok(argv[0], getpid());

    signal(SIGINT, cleanup);

    ENSURE_SUCCESS(sem = semaphore_get(key));

    /** Two procs per critical section */
    ENSURE_SUCCESS(semaphore_set_value(sem, 2));

    system("clear");
    for (i = 0; i < NUM_PROCS; ++i)
        if (fork() == 0)
            return aem();

    aem();

    return 0;
}

int aem(/* semaphore_t sem */) {
    while (1) {
        ENSURE_SUCCESS(semaphore_lock(sem));
        critical_section();
        ENSURE_SUCCESS(semaphore_unlock(sem));
    }
}

void cleanup(int s) {
    semaphore_destroy(sem);
    /** All other processes will die with ENSURE_SUCCESS()... dirty but just a
     * proof of concept */
}

void critical_section() {
    printf("<%d> cs start...", getpid());
    fflush(stdout);
    sleep(1 + getpid() % 10);
    printf("end\n");
}
