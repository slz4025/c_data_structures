#include <assert.h>
#include <pthread.h>

#include "../include/item.h"
#include "../include/list.h"
#include "../include/memory.h"
#include "../include/dict.h"
#include "../include/dict_conn.h"

struct _impl_dict_conn_t {
  dict_t *D;
  pthread_rwlock_t *rwlock;
};

dict_conn_t *dict_conn_create(dict_t *D) {
  dict_conn_t *DC = (dict_conn_t *) memory_malloc(sizeof(dict_conn_t));

  DC->D = D;
  DC->rwlock = memory_malloc(sizeof(pthread_rwlock_t));
  int ret = pthread_rwlock_init(DC->rwlock, NULL);
  if (ret != 0) { // could not make lock
    return NULL;
  }

  return DC;
}

void dict_conn_destroy(dict_conn_t *DC) {
  assert(DC);
  dict_destroy(DC->D);
  pthread_rwlock_destroy(DC->rwlock);
  memory_free(DC->rwlock);
  memory_free(DC);
}

size_t dict_conn_len(dict_conn_t *DC) {
  assert(DC);
  pthread_rwlock_rdlock(DC->rwlock);
  size_t len = dict_len(DC->D);
  pthread_rwlock_unlock(DC->rwlock);
  return len;
}

addr_t dict_conn_get(dict_conn_t *DC, addr_t k) {
  assert(DC);
  pthread_rwlock_rdlock(DC->rwlock);
  addr_t v = dict_get(DC->D, k);
  pthread_rwlock_unlock(DC->rwlock);
  return v;
}

list_t *dict_conn_items(dict_conn_t *DC) {
  assert(DC);
  pthread_rwlock_rdlock(DC->rwlock);
  list_t *items = dict_items(DC->D);
  pthread_rwlock_unlock(DC->rwlock);
  return items;
}

void dict_conn_set(dict_conn_t *DC, addr_t k, addr_t v) {
  assert(DC);
  pthread_rwlock_wrlock(DC->rwlock);
  dict_set(DC->D, k, v);
  pthread_rwlock_unlock(DC->rwlock);
}

item_t *dict_conn_del(dict_conn_t *DC, addr_t k) {
  assert(DC);
  pthread_rwlock_wrlock(DC->rwlock);
  item_t *I = dict_del(DC->D, k);
  pthread_rwlock_unlock(DC->rwlock);
  return I;
}
