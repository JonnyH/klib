#ifndef __KRWLOCK_H
#define __KRWLOCK_H

struct krwlock;

struct krwlock *krwlock_create(void);
void krwlock_destroy(struct krwlock *lock);

void krwlock_lock_read(struct krwlock *lock);
void krwlock_unlock_read(struct krwlock *lock);
void krwlock_lock_write(struct krwlock *lock);
void krwlock_unlock_write(struct krwlock *lock);

#endif /* __KRWLOCK_H */
