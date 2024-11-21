#ifndef LIST_CONN_H
#define LIST_CONN_H

#include "utils.h"
#include "list.h"

struct _impl_list_conn_t;
typedef struct _impl_list_conn_t list_conn_t;

list_conn_t *list_conn_create(list_t *L);
void list_conn_destroy(list_conn_t *LC);

// Thread-safe read functions
size_t list_conn_len(list_conn_t *LC);
addr_t list_conn_get(list_conn_t *LC, size_t i);

// Thread-safe write functions
void list_conn_set(list_conn_t *LC, size_t i, addr_t e);
addr_t list_conn_pop(list_conn_t *LC);
void list_conn_push(list_conn_t *LC, addr_t e);
void list_conn_insert(list_conn_t *LC, size_t i, addr_t e);
addr_t list_conn_remove(list_conn_t *LC, size_t i);

#endif

