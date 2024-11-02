#include <assert.h>
#include <pthread.h>

#include "../include/memory.h"
#include "../include/list.h"
#include "../include/list_conn.h"

struct _impl_list_conn_t {
  list_t *L;
  pthread_rwlock_t *rwlock;
};

list_conn_t *list_conn_create(list_t *L) {
  list_conn_t *LC = (list_conn_t *) memory_malloc(sizeof(list_conn_t));

  LC->L = L;
  LC->rwlock = memory_malloc(sizeof(pthread_rwlock_t));
  int ret = pthread_rwlock_init(LC->rwlock, NULL);
  if (ret != 0) { // could not make lock
    return NULL;
  }

  return LC;
}

void list_conn_destroy(list_conn_t *LC) {
  assert(LC);
  list_destroy(LC->L);
  pthread_rwlock_destroy(LC->rwlock);
  memory_free(LC->rwlock);
  memory_free(LC);
}

size_t list_conn_len(list_conn_t *LC) {
  assert(LC);
  pthread_rwlock_rdlock(LC->rwlock);
  size_t len = list_len(LC->L);
  pthread_rwlock_unlock(LC->rwlock);
  return len;
}

addr_t list_conn_get(list_conn_t *LC, size_t i) {
  assert(LC);
  pthread_rwlock_rdlock(LC->rwlock);
  addr_t e = list_get(LC->L, i);
  pthread_rwlock_unlock(LC->rwlock);
  return e;
}

void list_conn_set(list_conn_t *LC, size_t i, addr_t e) {
  assert(LC);
  pthread_rwlock_wrlock(LC->rwlock);
  list_set(LC->L, i, e);
  pthread_rwlock_unlock(LC->rwlock);
}

addr_t list_conn_pop(list_conn_t *LC) {
  assert(LC);
  pthread_rwlock_wrlock(LC->rwlock);
  addr_t e = list_pop(LC->L);
  pthread_rwlock_unlock(LC->rwlock);
  return e;
}

void list_conn_push(list_conn_t *LC, addr_t e) {
  assert(LC);
  pthread_rwlock_wrlock(LC->rwlock);
  list_push(LC->L, e);
  pthread_rwlock_unlock(LC->rwlock);
}

void list_conn_insert(list_conn_t *LC, size_t i, addr_t e) {
  assert(LC);
  pthread_rwlock_wrlock(LC->rwlock);
  list_insert(LC->L, i, e);
  pthread_rwlock_unlock(LC->rwlock);
}

addr_t list_conn_remove(list_conn_t *LC, size_t i) {
  assert(LC);
  pthread_rwlock_wrlock(LC->rwlock);
  addr_t e = list_remove(LC->L, i);
  pthread_rwlock_unlock(LC->rwlock);
  return e;
}
