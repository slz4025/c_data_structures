#include <assert.h>
#include <stdio.h>

#include "../lib/data_structures/list.h"
#include "../lib/data_structures/list_extended.h"
#include "../lib/data_structures/memory.h"

addr_t int_wrap(int i) {
  addr_t e = memory_malloc(sizeof(int));
  *((int *)e) = i;
  return e;
}

int int_unwrap(addr_t e) {
  assert(e);
  int i = *((int *) e);
  return i;
}

str_t int_str(addr_t e) {
  int i = int_unwrap(e);
  str_t s = (str_t) memory_calloc(INT_MAX_STR_SIZE + 1, sizeof(char));
  snprintf(s, INT_MAX_STR_SIZE, "%d", i);
  return s;
}

int main() {
  list_t *L = list_create(0);
  list_push(L, int_wrap(1));
  list_push(L, int_wrap(2));
  list_push(L, int_wrap(3));
  list_push(L, int_wrap(4));
  list_push(L, int_wrap(5));

  str_t s = list_string(L, int_str);
  printf("list: %s\n", s);
  list_total_destroy(L, memory_free);
}
