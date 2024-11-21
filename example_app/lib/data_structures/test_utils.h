#ifndef _TEST_UTILS_H
#define _TEST_UTILS_H

#include "utils.h"
#include "str.h"
#include "list.h"

addr_t int_wrap(int i);
int int_unwrap(addr_t e);
str_t int_str(addr_t e);
void int_sum(addr_t acc, addr_t e);
addr_t int_double(addr_t i);
bool int_eq(addr_t e1, addr_t e2);
size_t int_hash(addr_t e);
int int_compare(addr_t e1, addr_t e2);

str_t str_str(addr_t e);
void str_destroy(addr_t e);

struct _piece_t {
  char first;
  int second;
};
typedef struct _piece_t piece_t;
addr_t piece_create(char first, int second);
str_t piece_str(addr_t p);
addr_t piece_copy(addr_t p);
bool piece_eq(addr_t p1, addr_t p2);
size_t piece_hash(addr_t p);

str_t ptr_str(addr_t A);

#endif
