#include <assert.h>
#include <pthread.h>

#include "../include/item.h"
#include "../include/list.h"
#include "../include/memory.h"
#include "../include/set.h"
#include "../include/set_conn.h"

struct _impl_set_conn_t {
  set_t *S;
  pthread_rwlock_t *rwlock;
};

set_conn_t *set_conn_create(set_t *S) {
  set_conn_t *SC = (set_conn_t *) memory_malloc(sizeof(set_conn_t));

  SC->S = S;
  SC->rwlock = memory_malloc(sizeof(pthread_rwlock_t));
  int ret = pthread_rwlock_init(SC->rwlock, NULL);
  if (ret != 0) { // could not make lock
    return NULL;
  }

  return SC;
}

void set_conn_destroy(set_conn_t *SC) {
  assert(SC);
  set_destroy(SC->S);
  pthread_rwlock_destroy(SC->rwlock);
  memory_free(SC->rwlock);
  memory_free(SC);
}

size_t set_conn_len(set_conn_t *SC) {
  assert(SC);
  pthread_rwlock_rdlock(SC->rwlock);
  size_t len = set_len(SC->S);
  pthread_rwlock_unlock(SC->rwlock);
  return len;
}

bool set_conn_includes(set_conn_t *SC, addr_t e) {
  assert(SC);
  pthread_rwlock_rdlock(SC->rwlock);
  bool includes = set_includes(SC->S, e);
  pthread_rwlock_unlock(SC->rwlock);
  return includes;
}

list_t *set_conn_to_list(set_conn_t *SC) {
  assert(SC);
  pthread_rwlock_rdlock(SC->rwlock);
  list_t *entries = set_to_list(SC->S);
  pthread_rwlock_unlock(SC->rwlock);
  return entries;
}

void set_conn_add(set_conn_t *SC, addr_t e) {
  assert(SC);
  pthread_rwlock_wrlock(SC->rwlock);
  set_add(SC->S, e);
  pthread_rwlock_unlock(SC->rwlock);
}

addr_t set_conn_remove(set_conn_t *SC, addr_t e) {
  assert(SC);
  pthread_rwlock_wrlock(SC->rwlock);
  addr_t rem = set_remove(SC->S, e);
  pthread_rwlock_unlock(SC->rwlock);
  return rem;
}

