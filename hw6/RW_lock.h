#include <pthread.h>
#include <stdbool.h>

typedef struct RW_lock_s {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int num_readers_active;
  int num_writers_waiting;
  bool has_writer;
} RW_lock_t;

void RW_lock_init(RW_lock_t *lock);
     /* This routine should be called on a pointer to a struct variable of RW_lock_t
        to initialize it and ready it for use. */

void RW_read_lock(RW_lock_t *lock);
     /* This routine should be called at the beginning of a READER critical section */

void RW_read_unlock(RW_lock_t *lock);
     /* This routine should be called at the end of a READER critical section */

void RW_write_lock(RW_lock_t *lock);
     /* This routine should be called at the beginning of a WRITER critical section */

void RW_write_unlock(RW_lock_t *lock);
     /* This routine should be called at the end of a WRITER critical section */
