#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/test_utils.h"
#include "../include/memory.h"
#include "../include/str.h"
#include "../include/linked_list.h"

void test_basic_linked_list() {
  printf("basic linked list\n");

  link_t *L;
  str_t actual;
  str_t expected;
  addr_t e;

  // empty

  linked_list_t *LL = linked_list_create();
  assert(linked_list_empty(LL));
  assert(linked_list_len(LL) == 0);

  expected = "->";
  actual = linked_list_string(LL, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  // empty -> singleton

  e = int_wrap(0);
  L = linked_list_push(LL, e);
  assert(L->value == e);
  assert(!linked_list_empty(LL));
  assert(linked_list_len(LL) == 1);

  expected = "->0->";
  actual = linked_list_string(LL, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  // singleton -> multiple

  e = int_wrap(1);
  linked_list_push(LL, e);
  assert(!linked_list_empty(LL));
  assert(linked_list_len(LL) == 2);

  expected = "->0->1->";
  actual = linked_list_string(LL, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  // multiple -> singleton

  e = LL->join->value;
  linked_list_remove(LL, LL->join);
  assert(!linked_list_empty(LL));
  assert(linked_list_len(LL) == 1);
  memory_free(e);

  expected = "->1->";
  actual = linked_list_string(LL, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  // singleton -> empty

  e = LL->join->value;
  linked_list_remove(LL, LL->join);
  assert(linked_list_empty(LL));
  assert(linked_list_len(LL) == 0);
  memory_free(e);

  expected = "->";
  actual = linked_list_string(LL, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  linked_list_destroy(LL);
}

void test_combined_linked_list() {
  printf("combined linked list\n");

  linked_list_t *LL1;
  linked_list_t *LL2;
  linked_list_t *LL;

  str_t actual;
  str_t expected;
  addr_t e;

  // both empty

  LL1 = linked_list_create();
  LL2 = linked_list_create();
  LL = linked_list_combine(LL1, LL2);

  assert(linked_list_empty(LL));
  assert(linked_list_len(LL) == 0);

  expected = "->";
  actual = linked_list_string(LL, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  linked_list_destroy(LL1);
  linked_list_destroy(LL2);
  while (!linked_list_empty(LL)) {
    e = LL->join->value;
    linked_list_remove(LL, LL->join);
    memory_free(e);
  }
  linked_list_destroy(LL);

  // only second is empty

  LL1 = linked_list_create();
  e = int_wrap(0);
  linked_list_push(LL1, e);
  LL2 = linked_list_create();
  LL = linked_list_combine(LL1, LL2);

  assert(!linked_list_empty(LL));
  assert(linked_list_len(LL) == 1);

  expected = "->0->";
  actual = linked_list_string(LL, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  linked_list_destroy(LL1);
  linked_list_destroy(LL2);
  while (!linked_list_empty(LL)) {
    e = LL->join->value;
    linked_list_remove(LL, LL->join);
    memory_free(e);
  }
  linked_list_destroy(LL);

  // only first is empty

  LL1 = linked_list_create();
  LL2 = linked_list_create();
  e = int_wrap(1);
  linked_list_push(LL1, e);
  LL = linked_list_combine(LL1, LL2);

  assert(!linked_list_empty(LL));
  assert(linked_list_len(LL) == 1);

  expected = "->1->";
  actual = linked_list_string(LL, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  linked_list_destroy(LL1);
  linked_list_destroy(LL2);
  while (!linked_list_empty(LL)) {
    e = LL->join->value;
    linked_list_remove(LL, LL->join);
    memory_free(e);
  }
  linked_list_destroy(LL);

  // neither is empty small case

  LL1 = linked_list_create();
  e = int_wrap(0);
  linked_list_push(LL1, e);
  LL2 = linked_list_create();
  e = int_wrap(1);
  linked_list_push(LL1, e);
  LL = linked_list_combine(LL1, LL2);

  assert(!linked_list_empty(LL));
  assert(linked_list_len(LL) == 2);

  expected = "->0->1->";
  actual = linked_list_string(LL, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  linked_list_destroy(LL1);
  linked_list_destroy(LL2);
  while (!linked_list_empty(LL)) {
    e = LL->join->value;
    linked_list_remove(LL, LL->join);
    memory_free(e);
  }
  linked_list_destroy(LL);

  // neither is empty large case

  LL1 = linked_list_create();
  for (int i = 0; i < 8; i++) {
    if (i % 2 == 0) {
      continue;
    }
    e = int_wrap(i);
    linked_list_push(LL1, e);
  }

  LL2 = linked_list_create();
  for (int i = 0; i < 8; i++) {
    if (i % 2 == 1) {
      continue;
    }
    e = int_wrap(i);
    linked_list_push(LL2, e);
  }

  LL = linked_list_combine(LL1, LL2);

  assert(!linked_list_empty(LL));
  assert(linked_list_len(LL) == 8);

  expected = "->1->3->5->7->0->2->4->6->";
  actual = linked_list_string(LL, int_str);
  assert(strcmp(actual, expected) == 0);
  memory_free(actual);

  linked_list_destroy(LL1);
  linked_list_destroy(LL2);
  while (!linked_list_empty(LL)) {
    e = LL->join->value;
    linked_list_remove(LL, LL->join);
    memory_free(e);
  }
  linked_list_destroy(LL);
}

int main() {
  memory_pointers_init();

  test_basic_linked_list();
  test_combined_linked_list();

  str_t usage = memory_pointers_report();
  str_t expected = "->";
  assert(strcmp(usage, expected) == 0);
  memory_pointers_finish();

  return 0;
}
