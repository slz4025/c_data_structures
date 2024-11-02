#include <assert.h>
#include <stdio.h>

#include "../include/test_utils.h"
#include "../include/list_extended.h"
#include "../include/memory.h"
#include "../include/set.h"
#include "../include/set_extended.h"

void test_basic_set() {
  printf("basic set\n");

  str_t actual;
  str_t expected;
  bool is_in;
  addr_t e;

  addr_t e0 = int_wrap(0);
  addr_t e1 = int_wrap(1);
  addr_t e1copy = int_wrap(1);

  set_t *S = set_create(int_eq, int_hash);
  assert(set_len(S) == 0);

  expected = "{}";
  actual = set_string(S, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  is_in = set_includes(S, e0);
  assert(is_in == false);
  is_in = set_includes(S, e1);
  assert(is_in == false);

  set_add(S, e0);
  set_add(S, e1);
  set_add(S, e1copy);
  assert(set_len(S) == 2);

  expected = "{0,1}";
  actual = set_string(S, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  is_in = set_includes(S, e0);
  assert(is_in == true);
  is_in = set_includes(S, e1);
  assert(is_in == true);
  is_in = set_includes(S, e1copy);
  assert(is_in == true);

  e = set_remove(S, e0);
  assert(int_unwrap(e) == int_unwrap(e0));
  e = set_remove(S, e1);
  assert(int_unwrap(e) == int_unwrap(e1));
  e = set_remove(S, e1copy);
  assert(e == NULL);
  assert(set_len(S) == 0);

  expected = "{}";
  actual = set_string(S, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  is_in = set_includes(S, e0);
  assert(is_in == false);
  is_in = set_includes(S, e1);
  assert(is_in == false);

  memory_free(e0);
  memory_free(e1);
  memory_free(e1copy);
  set_destroy(S);
}

void test_set_from_list() {
  printf("set from list\n");

  list_t *L = list_create(0);

  addr_t e;

  for (int i = 0; i < 10; i++) {
    e = int_wrap(i % 5);
    list_push(L, e);
  }

  // S will store the first instance of each int value.
  set_t *S = set_from_list(L, int_eq, int_hash);
  assert(set_len(S) == 5);

  bool is_in;
  for (int i = 0; i < 5; i++) {
    e = int_wrap(i);
    is_in = set_includes(S, e);
    assert(is_in == true);
    memory_free(e);
  }

  set_destroy(S);
  list_total_destroy(L, memory_free);
}

void test_set_to_list() {
  printf("set to list\n");

  set_t *S = set_create(int_eq, int_hash);

  addr_t e;

  for (int i = 0; i < 5; i++) {
    e = int_wrap(i);
    set_add(S, e);
  }

  list_t *L = set_to_list(S);
  assert(list_len(L) == 5);

  list_t *L_sorted = list_sort(L, int_compare);
  for (int i = 0; i < 5; i++) {
    e = list_get(L_sorted, i);
    assert(int_unwrap(e) == i);
  }

  list_destroy(L_sorted);
  list_destroy(L);
  set_total_destroy(S, memory_free);
}

void test_set_copy() {
  printf("set copy\n");

  str_t actual;
  str_t expected;
  bool is_in;

  addr_t e0 = int_wrap(0);
  addr_t e1 = int_wrap(1);

  set_t *S = set_create(int_eq, int_hash);
  set_add(S, e0);
  set_add(S, e1);
  assert(set_len(S) == 2);

  set_t *C = set_copy(S);

  expected = "{0,1}";
  actual = set_string(C, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  is_in = set_includes(S, e0);
  assert(is_in == true);
  is_in = set_includes(S, e1);
  assert(is_in == true);

  memory_free(e0);
  memory_free(e1);
  set_destroy(S);
  set_destroy(C);
}

void test_set_deep_copy() {
  printf("set deep copy\n");

  str_t actual;
  str_t expected;

  addr_t e0 = piece_create('a',0);
  addr_t e1 = piece_create('b',1);

  set_t *S = set_create(piece_eq, piece_hash);
  set_add(S, e0);
  set_add(S, e1);
  assert(set_len(S) == 2);

  expected = "{(a,0),(b,1)}";
  actual = set_string(S, piece_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  set_t *C = set_deep_copy(S, piece_copy);

  expected = "{(a,0),(b,1)}";
  actual = set_string(C, piece_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  ((piece_t *) e0)->first = 'c';
  ((piece_t *) e1)->second = 2;

  expected = "{(c,0),(b,2)}";
  actual = set_string(S, piece_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  expected = "{(a,0),(b,1)}";
  actual = set_string(C, piece_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  memory_free(e0);
  memory_free(e1);
  set_destroy(S);
  set_total_destroy(C, memory_free);
}

void test_set_union() {
  printf("set union\n");

  addr_t e;

  set_t *S1 = set_create(int_eq, int_hash);
  for (int i = 0; i < 5; i++) {
    e = int_wrap(i);
    set_add(S1, e);
  }

  set_t *S2 = set_create(int_eq, int_hash);
  for (int i = 3; i < 8; i++) {
    e = int_wrap(i);
    set_add(S2, e);
  }

  set_t *U = set_union(S1, S2, int_eq, int_hash);
  assert(set_len(U) == 8);

  bool is_in;
  for (int i = 0; i < 8; i++) {
    e = int_wrap(i);
    is_in = set_includes(U, e);
    assert(is_in == true);
    memory_free(e);
  }

  set_total_destroy(S1, memory_free);
  set_total_destroy(S2, memory_free);
  set_destroy(U);
}

void test_set_intersection() {
  printf("set intersection\n");

  addr_t e;

  set_t *S1 = set_create(int_eq, int_hash);
  for (int i = 0; i < 5; i++) {
    e = int_wrap(i);
    set_add(S1, e);
  }

  set_t *S2 = set_create(int_eq, int_hash);
  for (int i = 3; i < 8; i++) {
    e = int_wrap(i);
    set_add(S2, e);
  }

  set_t *I = set_intersection(S1, S2, int_eq, int_hash);
  assert(set_len(I) == 2);

  bool is_in;
  for (int i = 0; i < 2; i++) {
    e = int_wrap(i+3);
    is_in = set_includes(I, e);
    assert(is_in == true);
    memory_free(e);
  }

  set_total_destroy(S1, memory_free);
  set_total_destroy(S2, memory_free);
  set_destroy(I);
}

void test_set_difference() {
  printf("set difference\n");

  addr_t e;

  set_t *S1 = set_create(int_eq, int_hash);
  for (int i = 0; i < 5; i++) {
    e = int_wrap(i);
    set_add(S1, e);
  }

  set_t *S2 = set_create(int_eq, int_hash);
  for (int i = 3; i < 8; i++) {
    e = int_wrap(i);
    set_add(S2, e);
  }

  set_t *D = set_difference(S1, S2, int_eq, int_hash);
  assert(set_len(D) == 3);

  bool is_in;
  for (int i = 0; i < 3; i++) {
    e = int_wrap(i);
    is_in = set_includes(D, e);
    assert(is_in == true);
    memory_free(e);
  }

  set_total_destroy(S1, memory_free);
  set_total_destroy(S2, memory_free);
  set_destroy(D);
}

int main() {
  memory_pointers_init();

  test_basic_set();
  test_set_to_list();
  test_set_from_list();
  test_set_copy();
  test_set_deep_copy();
  test_set_union();
  test_set_intersection();
  test_set_difference();

  str_t usage = memory_pointers_report();
  str_t expected = "->";
  assert(strcmp(usage, expected) == 0);
  memory_pointers_finish();

  return 0;
}
