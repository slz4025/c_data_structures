#ifndef HEAP_CONN_H
#define HEAP_CONN_H

#include "utils.h"
#include "item.h"
#include "list.h"
#include "heap.h"

struct _impl_heap_conn_t;
typedef struct _impl_heap_conn_t heap_conn_t;

heap_conn_t *heap_conn_create(heap_t *HC);
void heap_conn_destroy(heap_conn_t *HC);

// Thread-safe read functions
size_t heap_conn_len(heap_conn_t *HC);
item_t *heap_conn_peek_min(heap_conn_t *HC);

// Thread-safe write functions
void heap_conn_insert(heap_conn_t *HC, addr_t k, addr_t v);
void heap_conn_delete_min(heap_conn_t *HC);

#endif
