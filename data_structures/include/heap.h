#ifndef HEAP_H
#define HEAP_H

#include "utils.h"
#include "item.h"
#include "list.h"

struct _impl_heap_node_t;
typedef struct _impl_heap_node_t heap_node_t;

// Fibonnaci heap implementation with
// O(1) peek-min/insert/meld,
// O(1) amortized decrease-key
// and O(lgN) amortized pop-min.
struct _impl_heap_t;
typedef struct _impl_heap_t heap_t;

// compare should be a function that can create an ordering of keys,
// i.e. if k1 < k2 and k2 < k3, then k1 < k3.
heap_t *heap_create(
  int (*compare)(addr_t k1, addr_t k2),
  bool (*value_eq) (addr_t v1, addr_t v2),
  size_t (*value_hash) (addr_t v)
);
void heap_destroy(heap_t *H);

size_t heap_len(heap_t *H);
// Return a list of items with (value, (key, node)). O(N).
// Requires values to be unique.
// Useful before doing a decrease-key operation.
list_t *heap_iterator_create(heap_t *H);
void heap_iterator_destroy(list_t *iterator);
item_t *heap_peek_min(heap_t *H);

void heap_insert(heap_t *H, addr_t k, addr_t v);
void heap_decrease_key(heap_t *H, heap_node_t *N, addr_t k);
void heap_delete_min(heap_t *H);
heap_t *heap_meld(
  heap_t *H1,
  heap_t *H2,
  int (*compare)(addr_t k1, addr_t k2),
  bool (*value_eq) (addr_t v1, addr_t v2),
  size_t (*value_hash) (addr_t v)
);

str_t heap_string(heap_t *H, str_t (*key_string)(addr_t k), str_t (*value_string)(addr_t v));

#endif

