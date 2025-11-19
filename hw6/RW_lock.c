#include "RW_lock.h"
#include <pthread.h>

void RW_lock_init(RW_lock_t *const lock) {
  lock->num_readers_active = 0;
  lock->num_writers_waiting = 0;
  lock->has_writer = 0;
  pthread_cond_init(&lock->cond, NULL);
  pthread_mutex_init(&lock->mutex, NULL);
}

void RW_read_lock(RW_lock_t *const lock) {
  pthread_mutex_lock(&lock->mutex);

  while (lock->num_writers_waiting || lock->has_writer) {
    pthread_cond_wait(&lock->cond, &lock->mutex);
  }

  /*__sync_fetch_and_add(&lock->num_readers_active, 1);*/
  ++lock->num_readers_active;

  pthread_mutex_unlock(&lock->mutex);
}

void RW_read_unlock(RW_lock_t *const lock) {
  pthread_mutex_lock(&lock->mutex);

  /*if (!__sync_sub_and_fetch(&lock->num_readers_active, 1)) {*/
  if (!--lock->num_readers_active) {
    pthread_cond_signal(&lock->cond);
  }

  pthread_mutex_unlock(&lock->mutex);
}

void RW_write_lock(RW_lock_t *const lock) {
  pthread_mutex_lock(&lock->mutex);
  /*__sync_fetch_and_add(&lock->num_writers_waiting, 1);*/

  ++lock->num_writers_waiting;

  while (lock->num_readers_active || lock->has_writer) {
    pthread_cond_wait(&lock->cond, &lock->mutex);
  }

  // danger
  --lock->num_writers_waiting;
  lock->has_writer = true;

  pthread_mutex_lock(&lock->mutex);
}

void RW_write_unlock(RW_lock_t *lock) {
  pthread_mutex_lock(&lock->mutex);

  lock->has_writer = false;
  pthread_cond_signal(&lock->cond);

  pthread_mutex_unlock(&lock->mutex);
}
