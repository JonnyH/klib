#include "klib/krwlock.h"
#include "klib/katomic.h"

#include <pthread.h>
#include <stdlib.h>

struct krwlock
{
	pthread_mutex_t mutex;
	pthread_cond_t waiter;
	katomic_t read_count;
	katomic_t read_waiting;
	katomic_t write_count;
	katomic_t write_waiting;
};

struct krwlock* krwlock_create(void)
{
	struct krwlock *l = malloc(sizeof(struct krwlock));
	if (!l)
		goto err_out;
	if (pthread_mutex_init(&l->mutex, NULL))
		goto err_free;
	if (pthread_cond_init(&l->waiter, NULL))
		goto err_free_mutex;

	katomic_set(&l->read_count, 0);
	katomic_set(&l->read_waiting, 0);
	katomic_set(&l->write_count, 0);
	katomic_set(&l->write_waiting, 0);
	return l;

err_free_mutex:
	pthread_mutex_destroy(&l->mutex);
err_free:
	free(l);
err_out:
	return NULL;
}

void krwlock_destroy(struct krwlock* l)
{
	pthread_cond_destroy(&l->waiter);
	pthread_mutex_destroy(&l->mutex);
	free(l);
}

void krwlock_lock_read(struct krwlock* l)
{
	pthread_mutex_lock(&l->mutex);
	katomic_inc_return(&l->read_waiting);
	while (katomic_get(&l->write_count) != 0 ||
	       katomic_get(&l->write_waiting) != 0)
	{
		pthread_cond_wait(&l->waiter, &l->mutex);
	}
	katomic_inc_return(&l->read_count);
	katomic_dec_return(&l->read_waiting);
	pthread_mutex_unlock(&l->mutex);
}

void krwlock_unlock_read(struct krwlock* l)
{
	katomic_dec_return(&l->read_count);
	pthread_cond_broadcast(&l->waiter);
}

void krwlock_lock_write(struct krwlock* l)
{
	pthread_mutex_lock(&l->mutex);
	katomic_inc_return(&l->write_waiting);
	while (katomic_get(&l->write_count) != 0 ||
	       katomic_get(&l->read_count) != 0)
	{
		pthread_cond_wait(&l->waiter, &l->mutex);
	}
	katomic_inc_return(&l->write_count);
	katomic_dec_return(&l->write_waiting);
	pthread_mutex_unlock(&l->mutex);
}

void krwlock_unlock_write(struct krwlock *l)
{
	katomic_dec_return(&l->write_count);
	pthread_cond_broadcast(&l->waiter);
}
