#include <assert.h>
#include <stdio.h>

#include "../include/test_utils.h"
#include "../include/memory.h"

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

void int_sum(addr_t acc, addr_t e) {
  *((int *)acc) = int_unwrap(acc) + int_unwrap(e);
}

addr_t int_double(addr_t e) {
  int i = int_unwrap(e);
  int d = 2 * i;
  return int_wrap(d);
}

bool int_eq(addr_t e1, addr_t e2) {
  int i1 = *((int *) e1);
  int i2 = *((int *) e2);
  return i1 == i2;
}

size_t int_hash(addr_t e) {
  int i = *((int *) e);
  if (e < 0) {
    return -1 * i;
  } else {
    return i;
  }
}

int int_compare(addr_t e1, addr_t e2) {
  return int_unwrap(e2) - int_unwrap(e1);
}

str_t str_str(addr_t e) {
  str_t s = str_unwrap(e);
  size_t len = strlen(s);
  str_t n = (str_t) memory_calloc(len + 2 + 1, sizeof(char)); // +2 for quotes, +1 for \0
  strncat(n, "'", 1);
  strncat(n, s, len);
  strncat(n, "'", 1);
  return n;
}

void str_destroy(addr_t e) {
  str_t s = str_unwrap(e);
  memory_free(s);
  memory_free(e);
}

addr_t piece_create(char first, int second) {
  addr_t p = memory_malloc(sizeof(piece_t));
  ((piece_t *) p)->first = first;
  ((piece_t *) p)->second = second;
  return p;
}

str_t piece_str(addr_t p) {
  size_t len = 1 + INT_MAX_STR_SIZE + 1 + 2;
  str_t s = (str_t) memory_calloc(len + 1, sizeof(char)); // +1 for char, +1 for comma, +2 for parentheses, +1 for \0
  snprintf(s, len, "(%c,%d)", ((piece_t *) p)->first, ((piece_t *) p)->second);
  return s;
}

addr_t piece_copy(addr_t p) {
  addr_t c = memory_malloc(sizeof(piece_t));
  ((piece_t *) c)->first = ((piece_t *) p)->first;
  ((piece_t *) c)->second = ((piece_t *) p)->second;
  return c;
}

bool piece_eq(addr_t p1, addr_t p2) {
  if (((piece_t *) p1)->first != ((piece_t *) p2)->first) {
    return false;
  }
  if (((piece_t *) p1)->second != ((piece_t *) p2)->second) {
    return false;
  }
  return true;
}

size_t piece_hash(addr_t p) {
  int sum = (int) ((piece_t *) p)->first + ((piece_t *) p)->second;
  if (sum < 0) {
    return -1 * sum;
  } else {
    return sum;
  }
}

str_t ptr_str(addr_t A) {
  str_t s = (str_t) memory_calloc(PTR_MAX_STR_SIZE + 1, sizeof(char));
  snprintf(s, PTR_MAX_STR_SIZE, "%p", A);
  return s;
}
