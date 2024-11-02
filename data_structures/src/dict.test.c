#include <assert.h>
#include <stdio.h>

#include "../include/test_utils.h"
#include "../include/item.h"
#include "../include/memory.h"
#include "../include/dict.h"
#include "../include/dict_extended.h"

void test_basic_dict() {
  printf("basic dict\n");

  str_t actual;
  str_t expected;
  addr_t actual_value;
  list_t *items;
  list_t *keys;
  list_t *values;

  addr_t k0 = int_wrap(0);
  addr_t v0 = int_wrap(0);
  addr_t k1 = int_wrap(1);
  addr_t v1 = int_wrap(2);
  addr_t v1reset = int_wrap(3);

  dict_t *D = dict_create(int_eq, int_hash);
  assert(dict_len(D) == 0);

  expected = "{}";
  actual = dict_string(D, int_str, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  actual_value = dict_get(D, k0);
  assert(actual_value == NULL);
  actual_value = dict_get(D, k1);
  assert(actual_value == NULL);

  items = dict_items(D);
  assert(list_len(items) == 0);
  list_destroy(items);

  keys = dict_keys(D);
  assert(list_len(keys) == 0);
  list_destroy(keys);

  values = dict_values(D);
  assert(list_len(values) == 0);
  list_destroy(values);

  dict_set(D, k0, v0);
  dict_set(D, k1, v1);
  assert(dict_len(D) == 2);

  expected = "{0:0,1:2}";
  actual = dict_string(D, int_str, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  actual_value = dict_get(D, k0);
  assert(int_unwrap(actual_value) == int_unwrap(v0));
  actual_value = dict_get(D, k1);
  assert(int_unwrap(actual_value) == int_unwrap(v1));

  items = dict_items(D);
  assert(list_len(items) == 2);
  assert(int_unwrap(k0) == int_unwrap(item_get_key(list_get(items, 0))));
  assert(int_unwrap(v0) == int_unwrap(item_get_value(list_get(items, 0))));
  assert(int_unwrap(k1) == int_unwrap(item_get_key(list_get(items, 1))));
  assert(int_unwrap(v1) == int_unwrap(item_get_value(list_get(items, 1))));
  list_destroy(items);

  keys = dict_keys(D);
  assert(list_len(keys) == 2);
  assert(int_unwrap(k0) == int_unwrap(list_get(keys, 0)));
  assert(int_unwrap(k1) == int_unwrap(list_get(keys, 1)));
  list_destroy(keys);

  values = dict_values(D);
  assert(list_len(values) == 2);
  assert(int_unwrap(v0) == int_unwrap(list_get(values, 0)));
  assert(int_unwrap(v1) == int_unwrap(list_get(values, 1)));
  list_destroy(values);

  dict_set(D, k1, v1reset);
  assert(dict_len(D) == 2);

  expected = "{0:0,1:3}";
  actual = dict_string(D, int_str, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  actual_value = dict_get(D, k0);
  assert(int_unwrap(actual_value) == int_unwrap(v0));
  actual_value = dict_get(D, k1);
  assert(int_unwrap(actual_value) == int_unwrap(v1reset));

  items = dict_items(D);
  assert(list_len(items) == 2);
  assert(int_unwrap(k0) == int_unwrap(item_get_key(list_get(items, 0))));
  assert(int_unwrap(v0) == int_unwrap(item_get_value(list_get(items, 0))));
  assert(int_unwrap(k1) == int_unwrap(item_get_key(list_get(items, 1))));
  assert(int_unwrap(v1reset) == int_unwrap(item_get_value(list_get(items, 1))));
  list_destroy(items);

  keys = dict_keys(D);
  assert(list_len(keys) == 2);
  assert(int_unwrap(k0) == int_unwrap(list_get(keys, 0)));
  assert(int_unwrap(k1) == int_unwrap(list_get(keys, 1)));
  list_destroy(keys);

  values = dict_values(D);
  assert(list_len(values) == 2);
  assert(int_unwrap(v0) == int_unwrap(list_get(values, 0)));
  assert(int_unwrap(v1reset) == int_unwrap(list_get(values, 1)));
  list_destroy(values);

  addr_t I;
  I = dict_del(D, k0);
  item_destroy(I);
  I = dict_del(D, k1);
  item_destroy(I);
  I = dict_del(D, k1);
  assert(I == NULL);
  assert(dict_len(D) == 0);

  expected = "{}";
  actual = dict_string(D, int_str, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  actual_value = dict_get(D, k0);
  assert(actual_value == NULL);
  actual_value = dict_get(D, k1);
  assert(actual_value == NULL);

  items = dict_items(D);
  assert(list_len(items) == 0);
  list_destroy(items);

  keys = dict_keys(D);
  assert(list_len(keys) == 0);
  list_destroy(keys);

  values = dict_values(D);
  assert(list_len(values) == 0);
  list_destroy(values);

  dict_destroy(D);

  memory_free(k0);
  memory_free(v0);
  memory_free(k1);
  memory_free(v1);
  memory_free(v1reset);
}

void test_big_dict() {
  printf("big dict\n");

  dict_t *D = dict_create(int_eq, int_hash);

  addr_t I;
  addr_t k;
  addr_t v;

  size_t N = 100;
  for (int i = 0; i < N; i++) {
    k = int_wrap(i);
    v = int_wrap(2*i);
    dict_set(D, k, v);
  }

  for (int i = 0; i < N; i++) {
    k = int_wrap(i);
    v = dict_get(D, k);
    assert(int_unwrap(v) == 2*i);
    memory_free(k);
  }

  for (int i = 0; i < N; i++) {
    k = int_wrap(i);
    I = dict_del(D, k);
    memory_free(k);
    item_total_destroy(I, memory_free, memory_free);
  }
  dict_destroy(D);
}

void test_dict_copy() {
  printf("dict_t copy\n");

  str_t actual;
  str_t expected;
  addr_t actual_value;
  list_t *items;
  list_t *keys;
  list_t *values;

  addr_t k0 = int_wrap(0);
  addr_t v0 = int_wrap(0);
  addr_t k1 = int_wrap(1);
  addr_t v1 = int_wrap(2);

  dict_t *D = dict_create(int_eq, int_hash);
  dict_set(D, k0, v0);
  dict_set(D, k1, v1);
  assert(dict_len(D) == 2);

  dict_t *C = dict_copy(D);

  expected = "{0:0,1:2}";
  actual = dict_string(C, int_str, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  actual_value = dict_get(C, k0);
  assert(int_unwrap(actual_value) == int_unwrap(v0));
  actual_value = dict_get(C, k1);
  assert(int_unwrap(actual_value) == int_unwrap(v1));

  items = dict_items(C);
  assert(list_len(items) == 2);
  assert(int_unwrap(k0) == int_unwrap(item_get_key(list_get(items, 0))));
  assert(int_unwrap(v0) == int_unwrap(item_get_value(list_get(items, 0))));
  assert(int_unwrap(k1) == int_unwrap(item_get_key(list_get(items, 1))));
  assert(int_unwrap(v1) == int_unwrap(item_get_value(list_get(items, 1))));
  list_destroy(items);

  keys = dict_keys(C);
  assert(list_len(keys) == 2);
  assert(int_unwrap(k0) == int_unwrap(list_get(keys, 0)));
  assert(int_unwrap(k1) == int_unwrap(list_get(keys, 1)));
  list_destroy(keys);

  values = dict_values(C);
  assert(list_len(values) == 2);
  assert(int_unwrap(v0) == int_unwrap(list_get(values, 0)));
  assert(int_unwrap(v1) == int_unwrap(list_get(values, 1)));
  list_destroy(values);

  dict_destroy(D);
  memory_free(k0);
  memory_free(v0);
  memory_free(k1);
  memory_free(v1);

  dict_destroy(C);
}

void test_dict_deep_copy() {
  printf("dict_t deep copy\n");

  str_t actual;
  str_t expected;

  addr_t k0 = piece_create('a',0);
  addr_t v0 = piece_create('b',1);;
  addr_t k1 = piece_create('c',2);
  addr_t v1 = piece_create('d',3);

  dict_t *D = dict_create(piece_eq, piece_hash);
  dict_set(D, k0, v0);
  dict_set(D, k1, v1);
  assert(dict_len(D) == 2);

  expected = "{(a,0):(b,1),(c,2):(d,3)}";
  actual = dict_string(D, piece_str, piece_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  dict_t *C = dict_deep_copy(D, piece_copy, piece_copy);

  expected = "{(a,0):(b,1),(c,2):(d,3)}";
  actual = dict_string(C, piece_str, piece_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  ((piece_t *) k0)->first = 'e';
  ((piece_t *) v1)->second = 5;

  expected = "{(e,0):(b,1),(c,2):(d,5)}";
  actual = dict_string(D, piece_str, piece_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  expected = "{(a,0):(b,1),(c,2):(d,3)}";
  actual = dict_string(C, piece_str, piece_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  dict_destroy(D);
  memory_free(k0);
  memory_free(v0);
  memory_free(k1);
  memory_free(v1);

  dict_total_destroy(C, memory_free, memory_free);
}

int main() {
  memory_pointers_init();

  test_basic_dict();
  test_big_dict();
  test_dict_copy();
  test_dict_deep_copy();

  str_t usage = memory_pointers_report();
  str_t expected = "->";
  assert(strcmp(usage, expected) == 0);
  memory_pointers_finish();

  return 0;
}
