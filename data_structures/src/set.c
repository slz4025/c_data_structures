#include <assert.h>

#include "../include/dict.h"
#include "../include/dict_extended.h"
#include "../include/item.h"
#include "../include/list.h"
#include "../include/memory.h"
#include "../include/set.h"

const addr_t DEFAULT_VALUE = (addr_t) 1;

struct _impl_set_t {
  dict_t *D;
};

set_t *set_create(bool (*entry_eq) (addr_t e1, addr_t e2), size_t (*hash) (addr_t e)) {
  set_t *S = (set_t *) memory_malloc(sizeof(set_t));

  dict_t *D = dict_create(entry_eq, hash);
  S->D = D;

  return S;
}

void set_destroy(set_t *S) {
  assert(S);

  dict_destroy(S->D);
  memory_free(S); 
}

addr_t set_key_eq(set_t *S) {
  assert(S);

  return dict_key_eq(S->D);
}

addr_t set_hash(set_t *S) {
  assert(S);

  return dict_key_hash(S->D);
}

size_t set_len(set_t *S) {
  assert(S);

  size_t len = dict_len(S->D);
  return len;
}

bool set_includes(set_t *S, addr_t e) {
  assert(S);

  addr_t v = dict_get(S->D, e);
  return v != NULL;
}

list_t *set_to_list(set_t *S) {
  assert(S);

  list_t *keys = dict_keys(S->D);
  return keys;
}

void set_add(set_t *S, addr_t e) {
  assert(S);

  dict_set(S->D, e, DEFAULT_VALUE);
}

addr_t set_remove(set_t *S, addr_t e) {
  assert(S);

  item_t *I = dict_del(S->D, e);
  addr_t k = NULL;
  if (I != NULL) {
    k = item_get_key(I);
    item_destroy(I);
  }
  return k;
}
