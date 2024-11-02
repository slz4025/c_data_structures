#include <stdio.h>
#include <time.h>

#include "../include/test_utils.h"
#include "../include/memory.h"
#include "../include/heap.h"

void test_perf_performance() {
  int MAG = 4;
  heap_t *H;
  list_t *iterator;
  item_t *iteration;
  item_t *I;
  heap_node_t *HN;
  addr_t k;
  addr_t kn;
  addr_t v;
  size_t num_bytes_used;
  clock_t start;
  clock_t end;
  double duration;

  size_t N = 100000;

  for (int i = 0; i < MAG; i++) {
    printf("# ITEMS: %lu\n", N);

    memory_count_reset();

    start = clock();
    H = heap_create(int_compare, int_eq, int_hash);
    for (int i = 0; i < N; i++) {
      k = int_wrap(2*i);
      v = int_wrap(i);
      heap_insert(H, k, v);
    }
    end = clock();
    duration = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("INSERT SECS: %lf\n", duration);

    start = clock();
    iterator = heap_iterator_create(H);
    for (int i = 0; i < N; i++) {
      iteration = list_get(iterator, i);
      v = item_get_key(iteration);
      I = item_get_value(iteration);
      k = item_get_key(I);
      HN = item_get_value(I);
      kn = int_wrap(int_unwrap(k) / 2);
      heap_decrease_key(H, HN, kn);
      memory_free(k);
    }
    end = clock();
    duration = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("DECREASE-MIN SECS: %lf\n", duration);

    start = clock();
    for (int i = 0; i < N; i++) {
      I = heap_peek_min(H);
      k = item_get_key(I);
      v = item_get_value(I);
      heap_delete_min(H);
      memory_free(k);
      memory_free(v);
    }
    end = clock();
    duration = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("PEEK/DELETE-MIN SECS: %lf\n", duration);

    heap_destroy(H);

    num_bytes_used = memory_count_report();
    printf("# BYTES: %lu\n", num_bytes_used);

    N *= 2;
  }
}

int main() {
  test_perf_performance();

  return 0;
}
