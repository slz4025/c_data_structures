#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/memory.h"

void test_memory_pointers() {
  printf("memory pointers\n");

  addr_t p1;
  addr_t p2;
  str_t actual;
  str_t expected;

  memory_pointers_init();
  p1 = memory_malloc(sizeof(char));
  p2 = memory_calloc(10, sizeof(char));
  memory_free(p1);
  memory_free(p2);
  actual = memory_pointers_report();
  expected = "->";
  assert(strcmp(actual, expected) == 0);
  free(actual);
  memory_pointers_report();
  memory_pointers_finish();

  memory_pointers_init();
  p1 = memory_malloc(sizeof(char));
  p2 = memory_calloc(10, sizeof(char));
  actual = memory_pointers_report();
  size_t expected_len = 2 * 20 + 3 * 2;
  expected = (str_t) calloc(expected_len, sizeof(char));
  snprintf(expected, expected_len, "->%p->%p->", p1, p2);
  assert(strcmp(actual, expected) == 0);
  free(actual);
  free(expected);
  memory_pointers_finish();
}

int main() {
  test_memory_pointers();

  return 0;
}
