#include <assert.h>

#include "../include/memory.h"
#include "../include/linked_list.h"

link_t *link_create(addr_t e) {
  link_t *L = (link_t *) memory_malloc(sizeof(link_t));
  L->prev = NULL;
  L->next = NULL;
  L->value = e;
  return L;
}

void link_destroy(link_t *L) {
  memory_free(L);
}

linked_list_t *linked_list_create() {
  linked_list_t *LL = (linked_list_t *) memory_malloc(sizeof(linked_list_t));

  LL->join = NULL;

  return LL;
}

void linked_list_destroy(linked_list_t *LL) {
  assert(LL);

  memory_free(LL);
}

bool linked_list_empty(linked_list_t *LL) {
  return LL->join == NULL;
}

size_t linked_list_len(linked_list_t *LL) {
  assert(LL);

  size_t len = 0;

  if (linked_list_empty(LL)) {}
  else {
    link_t *curr;

    curr = LL->join;
    do {
      len += 1;
      curr = curr->next;
    } while (curr != LL->join);
  }

  return len;
}

str_t linked_list_string(linked_list_t *LL, str_t (*entry_string)(addr_t e)) {
  assert(LL);

  link_t *curr;
  addr_t e;
  str_t e_str;
  size_t len;

  size_t total_size = 0;
  if (!linked_list_empty(LL)) {
    curr = LL->join;
    do {
      e = curr->value;
      e_str = entry_string(e);
      len = strlen(e_str);
      memory_free(e_str);
      total_size += len + 2; // +2 for arrow
      curr = curr->next;
    } while (curr != LL->join);
  }

  str_t res = (str_t) memory_calloc(total_size + 2 + 1, sizeof(char)); // +2 for first arrow, +1 for \0

  strncat(res, "->", 2);

  if (!linked_list_empty(LL)) {
    curr = LL->join;
    do {
      e = curr->value;
      e_str = entry_string(e);
      len = strlen(e_str);
      strncat(res, e_str, len);
      memory_free(e_str);
      strncat(res, "->", 2);
      curr = curr->next;
    } while (curr != LL->join);
  }

  return res;
}

link_t *linked_list_push(linked_list_t *LL, addr_t e) {
  assert(LL);

  link_t *L = link_create(e);
  if (linked_list_empty(LL)) {
    LL->join = L;
    LL->join->prev = L;
    LL->join->next = L;
  } else {
    link_t *prev = LL->join->prev;
    LL->join->prev = L;
    L->next = LL->join;
    prev->next = L;
    L->prev = prev;
  }
  return L;
}

// Assume L is in LL.
void linked_list_remove(linked_list_t *LL, link_t *L) {
  assert(L);
  assert(LL);
  assert(!linked_list_empty(LL));

  if (L->next == L) {
    assert(LL->join == L);
    LL->join = NULL;
  } else {
    link_t *next = L->next;
    link_t *prev = L->prev;
    next->prev = prev;
    prev->next = next;
    if (LL->join == L) {
      LL->join = next;
    }
  }
  link_destroy(L);
}

linked_list_t *linked_list_combine(linked_list_t *LL1, linked_list_t *LL2) {
  assert(LL1);
  assert(LL2);

  linked_list_t *LL = linked_list_create();

  if (linked_list_empty(LL2)) {
    LL->join = LL1->join;
  } else if (linked_list_empty(LL1)) {
    LL->join = LL2->join;
  } else {
    link_t *head1 = LL1->join;
    link_t *tail1 = LL1->join->prev;
    link_t *head2 = LL2->join;
    link_t *tail2 = LL2->join->prev;
    head1->prev = tail2;
    tail2->next = head1;
    head2->prev = tail1;
    tail1->next = head2;
    LL->join = LL1->join;
  }

  return LL;
}
