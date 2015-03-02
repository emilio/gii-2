#ifndef SEMAPHORE_WRAPPER_H_
#define SEMAPHORE_WRAPPER_H_
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

typedef int semaphore_t;

/** Gets or creates a semaphore associated with `key` */
semaphore_t semaphore_get(key_t key);

/** Set value of the semaphore */
int semaphore_set_value(semaphore_t, int val);

/** Release the semaphore */
int semaphore_destroy(semaphore_t sem);

/** Lock and unlock the semaphore */
int semaphore_lock(semaphore_t sem);
int semaphore_unlock(semaphore_t sem);

/** Wait for the semaphore to be available, WITHOUT DECREMENTING */
int semaphore_wait(semaphore_t sem);
#endif
