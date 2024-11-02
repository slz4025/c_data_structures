#ifndef LIST_H
#define LIST_H

#include "utils.h"

struct _impl_list_t;
typedef struct _impl_list_t list_t;

list_t *list_create(size_t len);
void list_destroy(list_t *L);

size_t list_len(list_t *L);
addr_t list_get(list_t *L, size_t i);

void list_set(list_t *L, size_t i, addr_t e);
addr_t list_pop(list_t *L);
void list_push(list_t *L, addr_t e);
void list_insert(list_t *L, size_t i, addr_t e);
addr_t list_remove(list_t *L, size_t i);

#endif
