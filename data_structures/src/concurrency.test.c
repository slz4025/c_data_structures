#include <assert.h>
#include <stdio.h>

#include "../include/test_utils.h"
#include "../include/memory.h"
#include "../include/list.h"
#include "../include/dict.h"
#include "../include/set.h"
#include "../include/heap.h"
#include "../include/list_conn.h"
#include "../include/dict_conn.h"
#include "../include/set_conn.h"
#include "../include/heap_conn.h"

void test_list_conn() {
  printf("list conn\n");

  list_t *L = list_create(0);
  list_conn_t *LC = list_conn_create(L);
  list_conn_destroy(LC);
}

void test_dict_conn() {
  printf("dict conn\n");

  dict_t *D = dict_create(int_eq, int_hash);
  dict_conn_t *DC = dict_conn_create(D);
  dict_conn_destroy(DC);
}

void test_set_conn() {
  printf("set conn\n");

  set_t *S = set_create(int_eq, int_hash);
  set_conn_t *SC = set_conn_create(S);
  set_conn_destroy(SC);
}

void test_heap_conn() {
  printf("heap conn\n");

  heap_t *H = heap_create(int_compare, int_eq, int_hash);
  heap_conn_t *HC = heap_conn_create(H);
  heap_conn_destroy(HC);
}

int main() {
  memory_pointers_init();

  test_list_conn();
  test_dict_conn();
  test_set_conn();
  test_heap_conn();

  str_t usage = memory_pointers_report();
  str_t expected = "->";
  assert(strcmp(usage, expected) == 0);
  memory_pointers_finish();

  return 0;
}
