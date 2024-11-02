#ifndef DICT_H
#define DICT_H

#include "utils.h"
#include "item.h"
#include "list.h"

// Hash-table implementation with O(1) get/set/del.
struct _impl_dict_t;
typedef struct _impl_dict_t dict_t;

dict_t *dict_create(bool (*key_eq) (addr_t k1, addr_t k2), size_t (*key_hash) (addr_t k));
void dict_destroy(dict_t *D);

addr_t dict_key_eq(dict_t *D);
addr_t dict_key_hash(dict_t *D);
size_t dict_len(dict_t *D);
// If k does not exist in D, return NULL.
addr_t dict_get(dict_t *D, addr_t k);
list_t *dict_items(dict_t *D);

void dict_set(dict_t *D, addr_t k, addr_t v);
// If k does not exist in D, return NULL.
item_t *dict_del(dict_t *D, addr_t k);

#endif
