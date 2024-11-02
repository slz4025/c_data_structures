#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/memory.h"

struct __impl_link {
  struct __impl_link *prev;
  struct __impl_link *next;
  addr_t value;
};
typedef struct __impl_link _link;

_link *_link_create(addr_t e) {
  _link *L = (_link *) malloc(sizeof(_link));
  L->prev = NULL;
  L->next = NULL;
  L->value = e;
  return L;
}

void _link_destroy(_link *L) {
  free(L);
}

struct __impl_linked_list {
  _link *head;
  _link *tail;
};
typedef struct __impl_linked_list _linked_list;

_linked_list *_linked_list_create() {
  _linked_list *LL = (_linked_list *) malloc(sizeof(_linked_list));

  _link *head = _link_create(NULL);
  _link *tail = _link_create(NULL);
  head->next = tail;
  tail->prev = head;

  LL->head = head;
  LL->tail = tail;
  return LL;
}

void _linked_list_destroy(_linked_list *LL) {
  assert(LL);

  _link_destroy(LL->head);
  _link_destroy(LL->tail);
  free(LL);
}

size_t _linked_list_len(_linked_list *LL) {
  assert(LL);

  size_t len = 0;

  _link *curr;

  curr = LL->head->next;
  while (curr != LL->tail) {
    len += 1;
    curr = curr->next;
  }

  return len;
}

str_t _linked_list_string(_linked_list *LL, str_t (*entry_string)(addr_t e)) {
  assert(LL);

  _link *curr;
  addr_t e;
  str_t e_str;
  size_t len;

  size_t total_size = 0;
  curr = LL->head->next;
  while (curr != LL->tail) {
    e = curr->value;
    e_str = entry_string(e);
    len = strlen(e_str);
    free(e_str);
    total_size += len + 2; // +2 for arrow
    curr = curr->next;
  }

  str_t res = (str_t) calloc(total_size + 2 + 1, sizeof(char)); // +2 for first arrow, +1 for \0

  strncat(res, "->", 2);

  curr = LL->head->next;
  while (curr != LL->tail) {
    e = curr->value;
    e_str = entry_string(e);
    len = strlen(e_str);
    strncat(res, e_str, len);
    free(e_str);
    strncat(res, "->", 2);
    curr = curr->next;
  }

  return res;
}

void _linked_list_push(_linked_list *LL, addr_t e) {
  assert(LL);

  _link *new_link = _link_create(e);
  _link *prev = LL->tail->prev;

  new_link->next = LL->tail;
  LL->tail->prev = new_link;

  prev->next = new_link;
  new_link->prev = prev;
}

addr_t _linked_list_remove(_linked_list *LL, addr_t e, bool (*entry_eq) (addr_t e1, addr_t e2)) {
  assert(LL);

  _link *curr;
  addr_t c;

  curr = LL->head->next;
  while (curr != LL->tail) {
    c = curr->value;

    if (entry_eq(e, c)) {
        _link *next = curr->next;
        _link *prev = curr->prev;

        curr->prev->next = next;
        curr->next->prev = prev;

        _link_destroy(curr);
        break;
    } else {
      curr = curr->next;
    }
  }

  return c;
}

_linked_list *memory_pointers_record = NULL;
size_t total_memory_count = 0;

bool _addr_eq(addr_t A1, addr_t A2) {
  return A1 == A2;
}

const size_t ADDR_MAX_STR_SIZE = 20;

str_t _addr_str(addr_t A) {
  str_t s = (str_t) calloc(ADDR_MAX_STR_SIZE + 1, sizeof(char));
  snprintf(s, ADDR_MAX_STR_SIZE, "%p", A);
  return s;
}

void memory_pointers_init() {
  memory_pointers_record = _linked_list_create();
}

str_t memory_pointers_report() {
  assert(memory_pointers_record);

  str_t s = _linked_list_string(memory_pointers_record, _addr_str);
  return s;
}

void memory_pointers_finish() {
  assert(memory_pointers_record);

  _linked_list_destroy(memory_pointers_record);
  memory_pointers_record = NULL;
}

void memory_count_reset() {
  total_memory_count = 0;
}

size_t memory_count_report() {
  return total_memory_count;
}

addr_t memory_malloc(size_t num_bytes) {
  addr_t p = malloc(num_bytes);  
  assert(p);
  total_memory_count += num_bytes;
  if (memory_pointers_record != NULL) {
    _linked_list_push(memory_pointers_record, p);
  }
  return p;
}

addr_t memory_calloc(size_t num_entries, size_t num_bytes) {
  addr_t p = calloc(num_entries, num_bytes);  
  assert(p);
  total_memory_count += num_entries * num_bytes;
  if (memory_pointers_record != NULL) {
    _linked_list_push(memory_pointers_record, p);
  }
  return p;
}

void memory_free(addr_t p) {
  assert(p);
  if (memory_pointers_record != NULL) {
    _linked_list_remove(memory_pointers_record, p, _addr_eq);
  }
  free(p);
}
