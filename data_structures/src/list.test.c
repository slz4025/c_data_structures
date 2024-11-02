#include <assert.h>
#include <stdio.h>

#include "../include/test_utils.h"
#include "../include/memory.h"
#include "../include/str.h"
#include "../include/list.h"
#include "../include/list_extended.h"

list_t *setup(size_t len, int arr[]) {
  list_t *L = list_create(len);
  addr_t e;
  for (int i = 0; i < len; i++) {
    e = int_wrap(arr[i]);
    list_set(L, i, e);
  }
  return L;
}

void test_basic_list() {
  printf("basic list\n");

  int len;
  list_t *L;
  str_t actual;
  str_t expected;

  len = 10;
  int arr[] = {4, 9, 5, 5, 8, 7, 1, 6, 2, 3};
  expected = "[4,9,5,5,8,7,1,6,2,3]";
  L = setup(len, arr);
  actual = list_string(L, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_total_destroy(L, memory_free);

  len = 0;
  int empty_arr[] = {};
  expected = "[]";
  L = setup(len, empty_arr);
  actual = list_string(L, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_total_destroy(L, memory_free);

  L = list_create(10);
  assert(list_len(L) == 10);
  list_set(L, 0, int_wrap(1));
  list_set(L, 5, int_wrap(2));
  list_total_destroy(L, memory_free);
}

void test_stack_list() {
  printf("stack list\n");
  
  int len;
  list_t *L;
  list_t *S;
  str_t actual;
  str_t expected;
  addr_t e;

  len = 10;
  int arr[] = {4, 9, 5, 5, 8, 7, 1, 6, 2, 3};
  expected = "[4,9,5,5,8,7,1,6,2,3]";
  L = setup(len, arr);

  S = list_create(0);

  for (int i = 0; i < list_len(L); i++) {
    e = list_get(L, i);
    list_push(S, e);
  }

  assert(list_len(S) == list_len(L));
  actual = list_string(S, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  for (int i = 0; i < list_len(L); i++) {
    e = list_pop(S);
    assert(int_unwrap(e) == int_unwrap(list_get(L, list_len(L) - 1 - i)));
  }

  assert(list_len(S) == 0);
  actual = list_string(S, int_str);
  assert(strcmp(actual, "[]") == 0);
  memory_free(actual);

  list_destroy(S);
  list_total_destroy(L, memory_free);
}

void test_linked_list() {
  printf("linked list\n");

  int len;
  list_t *L;
  list_t *LL;
  str_t actual;
  str_t expected;
  addr_t e;

  len = 10;
  int arr[] = {4, 9, 5, 5, 8, 7, 1, 6, 2, 3};
  L = setup(len, arr);

  LL = list_create(0);

  for (int i = 0; i < list_len(L); i++) {
    list_insert(LL, 0, list_get(L, i));
  }

  assert(list_len(LL) == list_len(L));
  actual = list_string(LL, int_str);
  expected = "[3,2,6,1,7,8,5,5,9,4]";
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  for (int i = 0; i < list_len(L); i++) {
    e = list_remove(LL, 0);
    assert(int_unwrap(e) == int_unwrap(list_get(L, list_len(L) - 1 - i)));
  }

  assert(list_len(LL) == 0);
  actual = list_string(LL, int_str);
  assert(strcmp(actual, "[]") == 0);
  memory_free(actual);

  for (int i = 0; i < list_len(L); i++) {
    list_insert(LL, i, list_get(L, i));
  }

  assert(list_len(LL) == list_len(L));
  expected = "[4,9,5,5,8,7,1,6,2,3]";
  actual = list_string(LL, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  for (int i = list_len(L); i > 0; i--) {
    e = list_remove(LL, i-1);
    assert(int_unwrap(e) == int_unwrap(list_get(L, i - 1)));
  }

  assert(list_len(LL) == 0);
  actual = list_string(LL, int_str);
  assert(strcmp(actual, "[]") == 0);
  memory_free(actual);

  list_destroy(LL);
  list_total_destroy(L, memory_free);
}

void test_list_copy() {
  printf("list copy\n");

  size_t len;
  list_t *L;
  list_t *C;
  str_t actual;
  str_t expected;

  len = 10;
  int arr[] = {4, 9, 5, 5, 8, 7, 1, 6, 2, 3};
  L = setup(len, arr);
  expected = "[4,9,5,5,8,7,1,6,2,3]";
  C = list_copy(L);
  assert(list_len(C) == list_len(L));
  actual = list_string(C, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_destroy(C);
  list_total_destroy(L, memory_free);

  len = 0;
  int empty_arr[] = {};
  L = setup(len, empty_arr);
  expected = "[]";
  C = list_copy(L);
  assert(list_len(C) == list_len(L));
  actual = list_string(C, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_destroy(C);
  list_total_destroy(L, memory_free);
}

void test_list_deep_copy() {
  printf("list deep copy\n");

  addr_t p1 = piece_create('a', 1);
  addr_t p2 = piece_create('b', 2);

  str_t actual;
  str_t expected;

  list_t *L = list_create(2);
  list_set(L, 0, p1);
  list_set(L, 1, p2);

  actual = list_string(L, piece_str);
  expected = "[(a,1),(b,2)]";
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  list_t *C = list_deep_copy(L, piece_copy);

  actual = list_string(C, piece_str);
  expected = "[(a,1),(b,2)]";
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  ((piece_t *) p2)->second = 3;

  actual = list_string(L, piece_str);
  expected = "[(a,1),(b,3)]";
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  actual = list_string(C, piece_str);
  expected = "[(a,1),(b,2)]";
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  list_total_destroy(L, memory_free);
  list_total_destroy(C, memory_free);
}

void test_list_splice() {
  printf("list splice\n");

  size_t len;
  list_t *L;
  list_t *S;
  size_t slice_start;
  size_t slice_end;
  str_t actual;
  str_t expected;

  len = 10;
  int arr[] = {4, 9, 5, 5, 8, 7, 1, 6, 2, 3};
  L = setup(len, arr);

  slice_start = 2;
  slice_end = 4;
  expected = "[5,5]";
  S = list_splice(L, slice_start, slice_end);
  assert(list_len(S) == slice_end - slice_start);
  actual = list_string(S, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_destroy(S);

  slice_start = 2;
  slice_end = 2;
  expected = "[]";
  S = list_splice(L, slice_start, slice_end);
  assert(list_len(S) == slice_end - slice_start);
  actual = list_string(S, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_destroy(S);

  list_total_destroy(L, memory_free);
}

void test_list_map() {
  printf("list map\n");
  
  size_t len;
  list_t *L;
  addr_t (*map)(addr_t e);
  str_t actual;
  str_t expected;

  len = 10;
  int arr[] = {4, 9, 5, 5, 8, 7, 1, 6, 2, 3};
  L = setup(len, arr);
  expected = "[8,18,10,10,16,14,2,12,4,6]";
  map = int_double;
  list_t *M = list_map(L, map);
  assert(list_len(M) == list_len(L));
  actual = list_string(M, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_total_destroy(M, memory_free);
  list_total_destroy(L, memory_free);
}

void test_list_reduce() {
  printf("list reduce\n");

  size_t len;
  list_t *L;
  void (*combine)(addr_t acc, addr_t e);
  addr_t acc;
  int actual;
  int expected;

  len = 10;
  int arr[] = {4, 9, 5, 5, 8, 7, 1, 6, 2, 3};
  expected = 50;
  L = setup(len, arr);
  combine = int_sum;
  acc = int_wrap(0);
  list_reduce(L, combine, acc);
  actual = int_unwrap(acc);
  assert(actual == expected);
  memory_free(acc);
  list_total_destroy(L, memory_free);
}

void test_list_concat() {
  printf("list concat\n");

  size_t len;
  list_t *L1;
  list_t *L2;
  list_t *L;
  str_t actual;
  str_t expected;

  len = 5;
  int arr1[] = {0, 1, 2, 3, 4};
  L1 = setup(len, arr1);
  len = 5;
  int arr2[] = {5, 6, 7, 8, 9};
  L2 = setup(len, arr2);
  expected = "[0,1,2,3,4,5,6,7,8,9]";
  L = list_concat(L1, L2);
  actual = list_string(L, int_str);
  assert(list_len(L) == list_len(L1) + list_len(L2));
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_destroy(L);
  list_total_destroy(L2, memory_free);
  list_total_destroy(L1, memory_free);
}

void test_list_sort() {
  printf("list sort\n");

  size_t len;
  list_t *L;
  list_t *S;
  str_t actual;
  str_t expected;

  len = 10;
  int arr[] = {4, 9, 5, 5, 8, 7, 1, 6, 2, 3};
  L = setup(len, arr);
  expected = "[1,2,3,4,5,5,6,7,8,9]";
  S = list_sort(L, int_compare);
  assert(list_len(S) == list_len(L));
  actual = list_string(S, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_destroy(S);
  list_total_destroy(L, memory_free);
}

void test_list_unique() {
  printf("list unique\n");

  size_t len;
  list_t *L;
  list_t *U;
  str_t actual;
  str_t expected;

  len = 10;
  int arr[] = {4, 9, 5, 5, 8, 7, 1, 6, 2, 3};
  L = setup(len, arr);
  expected = "[1,2,3,4,5,6,7,8,9]";
  U = list_unique(L, int_compare);
  assert(list_len(U) == 9);
  actual = list_string(U, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);
  list_destroy(U);
  list_total_destroy(L, memory_free);
}

int main() {
  memory_pointers_init();

  test_basic_list();
  test_stack_list();
  test_linked_list();
  test_list_copy();
  test_list_deep_copy();
  test_list_splice();
  test_list_map();
  test_list_reduce();
  test_list_concat();
  test_list_sort();
  test_list_unique();

  str_t usage = memory_pointers_report();
  str_t expected = "->";
  assert(strcmp(usage, expected) == 0);
  memory_pointers_finish();

  return 0;
}

