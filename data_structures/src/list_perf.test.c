#include <stdio.h>
#include <time.h>

#include "../include/test_utils.h"
#include "../include/memory.h"
#include "../include/list.h"

void test_list_performance() {
  int MAG = 4;
  list_t *L;
  addr_t e;
  size_t num_bytes_used;
  clock_t start;
  clock_t end;
  double duration;

  size_t N = 1000000;

  for (int i = 0; i < MAG; i++) {
    start = clock();
    memory_count_reset();

    L = list_create(0);
    for (int i = 0; i < N; i++) {
      e = int_wrap(i);
      list_push(L, e);
    }
    for (int i = 0; i < N; i++) {
      e = list_pop(L);
      memory_free(e);
    }
    list_destroy(L);

    num_bytes_used = memory_count_report();
    end = clock();

    duration = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("# ITEMS: %lu\n", N);
    printf("SECS: %lf\n", duration);
    printf("# BYTES: %lu\n", num_bytes_used);

    N *= 2;
  }
}

int main() {
  test_list_performance();

  return 0;
}
