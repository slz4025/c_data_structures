#ifndef LIST_EXTENDED_H
#define LIST_EXTENDED_H

#include "list.h"

void list_total_destroy(list_t *L, void (*entry_destroy)(addr_t e));
str_t list_string(list_t *L, str_t (*entry_string)(addr_t e));

list_t *list_copy(list_t *L);
list_t *list_deep_copy(list_t *L, addr_t (*deep_copy)(addr_t e));
list_t *list_splice(list_t *L, size_t start, size_t end);

/* compare needs to be able
 * to handle NULL.
 */
list_t *list_sort(list_t *L, int (*compare)(addr_t e1, addr_t e2));
list_t *list_unique(list_t *L, int (*compare)(addr_t e1, addr_t e2));

list_t *list_concat(list_t *L1, list_t *L2);
list_t *list_map(list_t *L, addr_t (*map)(addr_t e));
void list_for_each(list_t *L, void (*action)(addr_t e));
void list_reduce(list_t *L, void (*combine)(addr_t acc, addr_t e), addr_t acc);

#endif
