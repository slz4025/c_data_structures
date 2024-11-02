#include <assert.h>
#include <pthread.h>

#include "../include/item.h"
#include "../include/list.h"
#include "../include/memory.h"
#include "../include/heap.h"
#include "../include/heap_conn.h"

struct _impl_heap_conn_t {
  heap_t *H;
  pthread_rwlock_t *rwlock;
};

heap_conn_t *heap_conn_create(heap_t *H) {
  heap_conn_t *HC = (heap_conn_t *) memory_malloc(sizeof(heap_conn_t));

  HC->H = H;
  HC->rwlock = memory_malloc(sizeof(pthread_rwlock_t));
  int ret = pthread_rwlock_init(HC->rwlock, NULL);
  if (ret != 0) { // could not make lock
    return NULL;
  }

  return HC;
}

void heap_conn_destroy(heap_conn_t *HC) {
  assert(HC);
  heap_destroy(HC->H);
  pthread_rwlock_destroy(HC->rwlock);
  memory_free(HC->rwlock);
  memory_free(HC);
}

size_t heap_conn_len(heap_conn_t *HC) {
  assert(HC);
  pthread_rwlock_rdlock(HC->rwlock);
  size_t len = heap_len(HC->H);
  pthread_rwlock_unlock(HC->rwlock);
  return len;
}

item_t *heap_conn_peek_min(heap_conn_t *HC) {
  assert(HC);
  pthread_rwlock_rdlock(HC->rwlock);
  item_t *I = heap_peek_min(HC->H);
  pthread_rwlock_unlock(HC->rwlock);
  return I;
}

void heap_conn_insert(heap_conn_t *HC, addr_t k, addr_t v) {
  assert(HC);
  pthread_rwlock_wrlock(HC->rwlock);
  heap_insert(HC->H, k, v);
  pthread_rwlock_unlock(HC->rwlock);
}

void heap_conn_delete_min(heap_conn_t *HC) {
  assert(HC);
  pthread_rwlock_wrlock(HC->rwlock);
  heap_delete_min(HC->H);
  pthread_rwlock_unlock(HC->rwlock);
}
