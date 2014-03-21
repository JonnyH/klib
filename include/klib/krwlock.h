#ifndef __KRWLOCK_H
#define __KRWLOCK_H

#include "katomic.h"

#include <stdbool.h>
#include <pthread.h>


struct krwlock
{
	pthread_mutex_t mutex;
	pthread_cond_t waiter;
	katomic_t read_count;
	katomic_t read_waiting;
	katomic_t write_count;
	katomic_t write_waiting;
};


static inline bool krwlock_init(struct krwlock *lock)
{
	if (pthread_mutex_init(&lock->mutex, NULL))
		goto err_out;
	if (pthread_cond_init(&lock->waiter, NULL))
		goto err_free_mutex;

	katomic_set(&lock->read_count, 0);
	katomic_set(&lock->read_waiting, 0);
	katomic_set(&lock->write_count, 0);
	katomic_set(&lock->write_waiting, 0);
	return true;

	err_free_mutex:
	pthread_mutex_destroy(&lock->mutex);
	err_out:
	return false;
}

static inline void krwlock_destroy(struct krwlock *lock)
{
	pthread_mutex_destroy(&lock->mutex);
	pthread_cond_destroy(&lock->waiter);
	katomic_set(&lock->read_count, 0);
	katomic_set(&lock->read_waiting, 0);
	katomic_set(&lock->write_count, 0);
	katomic_set(&lock->write_waiting, 0);
}

static inline void krwlock_lock_read(struct krwlock *lock)
{
	pthread_mutex_lock(&lock->mutex);
	katomic_inc_return(&lock->read_waiting);
	while (katomic_get(&lock->write_count) != 0 ||
		   katomic_get(&lock->write_waiting) != 0)
	{
		pthread_cond_wait(&lock->waiter, &lock->mutex);
	}
	katomic_inc_return(&lock->read_count);
	katomic_dec_return(&lock->read_waiting);
	pthread_mutex_unlock(&lock->mutex);
}

static inline void krwlock_unlock_read(struct krwlock *lock)
{
	katomic_dec_return(&lock->read_count);
	pthread_cond_broadcast(&lock->waiter);
}

static inline void krwlock_lock_write(struct krwlock *lock)
{
	pthread_mutex_lock(&lock->mutex);
	katomic_inc_return(&lock->write_waiting);
	while (katomic_get(&lock->write_count) != 0 ||
		   katomic_get(&lock->read_count) != 0)
	{
		pthread_cond_wait(&lock->waiter, &lock->mutex);
	}
	katomic_inc_return(&lock->write_count);
	katomic_dec_return(&lock->write_waiting);
	pthread_mutex_unlock(&lock->mutex);
}

static inline void krwlock_unlock_write(struct krwlock *lock)
{
	katomic_dec_return(&lock->write_count);
	pthread_cond_broadcast(&lock->waiter);
}

#endif /* __KRWLOCK_H */
