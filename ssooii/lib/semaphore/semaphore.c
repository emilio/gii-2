#include "semaphore.h"

semaphore_t semaphore_get(key_t key) {
	return semget(key, 1, 0600 | IPC_CREAT);
}

int semaphore_set_value(semaphore_t sem, int val) {
	return semctl(sem, 0, SETVAL, val);
}

int semaphore_lock(semaphore_t sem) {
	static struct sembuf decrement_op = {0, -1, 0};

	return semop(sem, &decrement_op, 1);
}

int semaphore_unlock(semaphore_t sem) {
	static struct sembuf increment_op = {0, 1, 0};

	return semop(sem, &increment_op, 1);
}

int semaphore_wait(semaphore_t sem) {
	static struct sembuf wait_op = {0, 0, 0};

	return semop(sem, &wait_op, 1);
}

int semaphore_destroy(semaphore_t sem) {
	return semctl(sem, 0, IPC_RMID);
}
