#include <assert.h>
#include <stdio.h>

#include "../include/test_utils.h"
#include "../include/dict.h"
#include "../include/item.h"
#include "../include/memory.h"
#include "../include/heap.h"

void test_basic_heap() {
  printf("basic heap\n");

  heap_t *H = heap_create(int_compare, int_eq, int_hash);
  assert(heap_len(H) == 0);

  //str_t state;

  item_t *I;
  addr_t k;
  addr_t v;

  I = heap_peek_min(H);
  assert(I == NULL);

  for (int i = 10; i > 0; i--) {
    k = int_wrap(i);
    v = int_wrap(i);
    heap_insert(H, k, v);
    assert(heap_len(H) == 10 - i + 1);
  }

  //printf("heap insert state:\n");
  //state = heap_string(H, int_str, int_str);
  //printf("%s\n", state);
  //memory_free(state);

  for (int i = 1; i <= 10; i++) {
    I = heap_peek_min(H);
    assert(I);
    k = item_get_key(I);
    v = item_get_value(I);
    assert(int_unwrap(k) == i);
    assert(int_unwrap(v) == i);

    heap_delete_min(H);
    assert(heap_len(H) == 10 - i);

    memory_free(k);
    memory_free(v);
  }

  heap_destroy(H);
}

void test_heap_meld() {
  printf("heap meld\n");
  
  //str_t state;

  heap_t *H1;
  heap_t *H2;
  heap_t *H;

  item_t *I;
  addr_t k;
  addr_t v;

  // both are empty

  H1 = heap_create(int_compare, int_eq, int_hash);
  H2 = heap_create(int_compare, int_eq, int_hash);

  H = heap_meld(H1, H2, int_compare, int_eq, int_hash);
  assert(heap_len(H) == 0);

  //printf("heap meld both are empty state:\n");
  //state = heap_string(H, int_str, int_str);
  //printf("%s\n", state);
  //memory_free(state);

  heap_destroy(H1);
  heap_destroy(H2);
  heap_destroy(H);

  // heap_t 1 is empty

  H1 = heap_create(int_compare, int_eq, int_hash);
  H2 = heap_create(int_compare, int_eq, int_hash);

  for (int i = 0; i < 4; i++) {
    k = int_wrap(i);
    v = int_wrap(i);
    heap_insert(H2, k, v);
  }

  H = heap_meld(H1, H2, int_compare, int_eq, int_hash);
  assert(heap_len(H) == 4);

  //printf("heap meld heap_t 1 is empty state:\n");
  //state = heap_string(H, int_str, int_str);
  //printf("%s\n", state);
  //memory_free(state);

  for (int i = 0; i < 4; i++) {
    I = heap_peek_min(H); 
    k = item_get_key(I);
    v = item_get_value(I);
    assert(int_unwrap(k) == i);
    assert(int_unwrap(v) == i);
    heap_delete_min(H);
    memory_free(k);
    memory_free(v);
  }

  heap_destroy(H1);
  heap_destroy(H2);
  heap_destroy(H);

  // heap_t 2 is empty

  H1 = heap_create(int_compare, int_eq, int_hash);
  H2 = heap_create(int_compare, int_eq, int_hash);

  for (int i = 0; i < 4; i++) {
    k = int_wrap(i);
    v = int_wrap(i);
    heap_insert(H1, k, v);
  }

  H = heap_meld(H1, H2, int_compare, int_eq, int_hash);
  assert(heap_len(H) == 4);

  //printf("heap meld heap_t 2 is empty state:\n");
  //state = heap_string(H, int_str, int_str);
  //printf("%s\n", state);
  //memory_free(state);

  for (int i = 0; i < 4; i++) {
    I = heap_peek_min(H); 
    k = item_get_key(I);
    v = item_get_value(I);
    assert(int_unwrap(k) == i);
    assert(int_unwrap(v) == i);
    heap_delete_min(H);
    memory_free(k);
    memory_free(v);
  }

  heap_destroy(H1);
  heap_destroy(H2);
  heap_destroy(H);

  // different elements
  H1 = heap_create(int_compare, int_eq, int_hash);
  H2 = heap_create(int_compare, int_eq, int_hash);

  for (int i = 0; i < 8; i++) {
    if (i % 2 == 0) {
      continue;
    }
    k = int_wrap(i);
    v = int_wrap(i);
    heap_insert(H1, k, v);
  }

  for (int i = 0; i < 8; i++) {
    if (i % 2 == 1) {
      continue;
    }
    k = int_wrap(i);
    v = int_wrap(i);
    heap_insert(H2, k, v);
  }

  H = heap_meld(H1, H2, int_compare, int_eq, int_hash);
  assert(heap_len(H) == 8);

  //printf("heap meld different elements state:\n");
  //state = heap_string(H, int_str, int_str);
  //printf("%s\n", state);
  //memory_free(state);

  for (int i = 0; i < 8; i++) {
    I = heap_peek_min(H); 
    k = item_get_key(I);
    v = item_get_value(I);
    assert(int_unwrap(k) == i);
    assert(int_unwrap(v) == i);
    heap_delete_min(H);
    memory_free(k);
    memory_free(v);
  }

  heap_destroy(H1);
  heap_destroy(H2);
  heap_destroy(H);
  
  // same elements
  H1 = heap_create(int_compare, int_eq, int_hash);
  H2 = heap_create(int_compare, int_eq, int_hash);

  for (int i = 0; i < 5; i++) {
    k = int_wrap(i);
    v = int_wrap(i);
    heap_insert(H1, k, v);
  }

  for (int i = 0; i < 5; i++) {
    k = int_wrap(i);
    v = int_wrap(i);
    heap_insert(H2, k, v);
  }

  H = heap_meld(H1, H2, int_compare, int_eq, int_hash);
  assert(heap_len(H) == 10);

  //printf("heap meld same elements state:\n");
  //state = heap_string(H, int_str, int_str);
  //printf("%s\n", state);
  //memory_free(state);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 2; j++) {
      I = heap_peek_min(H); 
      k = item_get_key(I);
      v = item_get_value(I);
      assert(int_unwrap(k) == i);
      assert(int_unwrap(v) == i);
      heap_delete_min(H);
      memory_free(k);
      memory_free(v);
    }
  }

  heap_destroy(H1);
  heap_destroy(H2);
  heap_destroy(H);
}

void test_heap_decrease_key() {
  printf("heap decrease key\n");

  heap_t *H = heap_create(int_compare, int_eq, int_hash);

  list_t *iterator;
  item_t *iteration;
  item_t *I;
  heap_node_t *N;
  addr_t k;
  addr_t kn;
  addr_t v;

  for (int i = 0; i < 5; i++) {
    k = int_wrap(2*i);
    v = int_wrap(2*i);
    heap_insert(H, k, v);
  }

  iterator = heap_iterator_create(H);
  for (int i = 0; i < list_len(iterator); i++) {
    iteration = list_get(iterator, i);
    v = item_get_key(iteration);
    I = item_get_value(iteration);
    k = item_get_key(I);
    N = item_get_value(I);
    kn = int_wrap(int_unwrap(k) / 2);
    heap_decrease_key(H, N, kn);
    memory_free(k);
  }
  heap_iterator_destroy(iterator);

  for (int i = 0; i < 5; i++) {
    I = heap_peek_min(H); 
    k = item_get_key(I);
    v = item_get_value(I);
    assert(int_unwrap(k) == i);
    assert(int_unwrap(v) == 2*i);
    heap_delete_min(H);
    memory_free(k);
    memory_free(v);
  }

  heap_destroy(H);
}

int main() {
  memory_pointers_init();

  test_basic_heap();
  test_heap_meld();
  test_heap_decrease_key();

  str_t usage = memory_pointers_report();
  str_t expected = "->";
  assert(strcmp(usage, expected) == 0);
  memory_pointers_finish();

  return 0;
}

