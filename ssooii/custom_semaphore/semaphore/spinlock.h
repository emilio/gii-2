#ifndef SPINLOCK_H_
#define SPINLOCK_H_

typedef volatile sig_atomic_t spinlock_t;

#define spinlock_init spinlock_release
#define spinlock_acquire(spinlock_ref) while ( ! __sync_bool_compare_and_swap(spinlock_ref, 0, 1) ) {}
#define spinlock_release(spinlock_ref) (*spinlock_ref = 0)
#endif
