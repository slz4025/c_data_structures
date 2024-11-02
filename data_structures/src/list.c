#include <assert.h>

#include "../include/memory.h"
#include "../include/list.h"

struct _impl_list_t {
  size_t len;
  // If len == 0, capacity == 0.
  // Else, capacity / 2 < len <= capacity.
  size_t capacity;
  addr_t *arr;
};

void _list_resize(list_t *L, size_t new_len) {
  if (new_len == 0) {
    L->len = 0;
    L->capacity = 0;
    addr_t *arr = L->arr;
    if (arr != NULL) {
      memory_free(arr);
    }
    L->arr = NULL;
  } else {
    if (L->capacity / 2 < new_len && new_len <= L->capacity) {
      L->len = new_len;
      return;
    }

    size_t new_capacity;
    if (L->capacity == 0) {
      new_capacity = 1;
    } else {
      new_capacity = L->capacity;
    }
    while (new_capacity < new_len) {
      new_capacity *= 2;
    } 
    while (new_capacity >= 2 * new_len) {
      new_capacity /= 2;
    }

    addr_t *arr = (addr_t *) memory_calloc(new_capacity, sizeof(addr_t));

    if (L->arr != NULL) {
      addr_t e;
      size_t copy_len = L->len;
      if (new_capacity < copy_len) {
        copy_len = new_capacity;
      }
      for (int i = 0; i < copy_len; i++) {
        e = list_get(L, i);
        *(arr + i) = e;
      }  
      memory_free(L->arr);
    }
    L->len = new_len;
    L->capacity = new_capacity;
    L->arr = arr;
  }
}

list_t *list_create(size_t len) {
  list_t *L = (list_t *) memory_malloc(sizeof(list_t));

  L->len = 0;
  L->capacity = 0;
  L->arr = NULL;

  _list_resize(L, len);

  return L;
}

void list_destroy(list_t *L) {
  assert(L);

  if (L->arr != NULL) {
    memory_free(L->arr);
  }
  memory_free(L);
}

size_t list_len(list_t *L) {
  assert(L);

  size_t len = L->len;
  return len;
}


addr_t list_get(list_t *L, size_t i) {
  assert(L);

  assert(i < L->len);
  addr_t e = *(L->arr + i);
  return e;
}

void list_set(list_t *L, size_t i, addr_t e) {
  assert(L);

  assert(i < L->len);
  *(L->arr + i) = e;
}

addr_t list_pop(list_t *L) {
  assert(L);

  assert(L->len > 0);
  addr_t e = list_get(L, L->len - 1);
  _list_resize(L, L->len - 1);
  return e;
}

void list_push(list_t *L, addr_t e) {
  assert(L);

  _list_resize(L, L->len + 1);
  list_set(L, L->len - 1, e);
}

void list_insert(list_t *L, size_t i, addr_t e) {
  assert(L);

  assert(i <= L->len);

  _list_resize(L, L->len + 1);

  addr_t t;
  for (int j = L->len - 1; j > i; j--) {
     t = list_get(L, j-1);
     list_set(L, j, t);
  }
  list_set(L, i, e);
}

addr_t list_remove(list_t *L, size_t i) {
  assert(L);

  assert(i < L->len);

  addr_t e = list_get(L, i);

  addr_t t;
  for (int j = i+1; j < L->len; j++) {
     t = list_get(L, j);
     list_set(L, j-1, t);
  }
  _list_resize(L, L->len - 1);

  return e;
}
