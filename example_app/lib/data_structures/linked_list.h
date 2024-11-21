#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "utils.h"

struct _impl_link_t {
  struct _impl_link_t *prev;
  struct _impl_link_t *next;
  addr_t value;
};
typedef struct _impl_link_t link_t;

link_t *link_create(addr_t e);
void link_destroy(link_t *L);

// Circular doubly-linked list.
struct impl_linked_list_t {
  // If no entries, NULL, else will point to the element that represents
  // beginning and end of linked list.
  link_t *join;
};
typedef struct impl_linked_list_t linked_list_t;

linked_list_t *linked_list_create();
void linked_list_destroy(linked_list_t *LL);

bool linked_list_empty(linked_list_t *LL);
size_t linked_list_len(linked_list_t *LL);
str_t linked_list_string(linked_list_t *LL, str_t (*entry_string)(addr_t e));

link_t *linked_list_push(linked_list_t *LL, addr_t e);
void linked_list_remove(linked_list_t *LL, link_t *L);
linked_list_t *linked_list_combine(linked_list_t *LL1, linked_list_t *LL2);

#endif
