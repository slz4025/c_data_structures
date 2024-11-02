#include <assert.h>

#include "../include/memory.h"
#include "../include/item.h"

struct _item_t {
  addr_t key;
  addr_t value;
};

item_t *item_create(addr_t k, addr_t v) {
  item_t *I = (item_t *) memory_malloc(sizeof(item_t));
  I->key = k;
  I->value = v;
  return I;
}

void item_destroy(item_t *I) {
  assert(I);

  memory_free(I);
}

void item_total_destroy(item_t *I, void (*key_destroy)(addr_t k), void (*value_destroy)(addr_t v)) {
  assert(I);

  addr_t k = item_get_key(I);
  addr_t v = item_get_value(I);
  key_destroy(k);
  value_destroy(v);
  item_destroy(I);
}

addr_t item_get_key(item_t *I) {
  assert(I);

  return I->key;  
}

addr_t item_get_value(item_t *I) {
  assert(I);

  return I->value; 
}

void item_set_key(item_t *I, addr_t k) {
  assert(I);

  I->key = k;
}

void item_set_value(item_t *I, addr_t v) {
  assert(I);

  I->value = v;
}
