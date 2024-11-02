#ifndef ITEM_H
#define ITEM_H

#include "utils.h"

struct _item_t;
typedef struct _item_t item_t;

item_t *item_create(addr_t k, addr_t v);

void item_destroy(item_t *I);
void item_total_destroy(item_t *I, void (*key_destroy)(addr_t k), void (*value_destroy)(addr_t v));

addr_t item_get_key(item_t *I);
addr_t item_get_value(item_t *I);
void item_set_key(item_t *I, addr_t k);
void item_set_value(item_t *I, addr_t v);

#endif
