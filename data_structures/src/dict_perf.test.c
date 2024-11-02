#include <stdio.h>
#include <time.h>

#include "../include/test_utils.h"
#include "../include/item.h"
#include "../include/memory.h"
#include "../include/dict.h"

void test_dict_performance() {
  int MAG = 4;
  dict_t *D;
  item_t *I;
  addr_t k;
  addr_t v;
  size_t num_bytes_used;
  clock_t start;
  clock_t end;
  double duration;

  size_t N = 100000;

  for (int i = 0; i < MAG; i++) {
    start = clock();
    memory_count_reset();

    D = dict_create(int_eq, int_hash);
    for (int i = 0; i < N; i++) {
      k = int_wrap(i);
      v = int_wrap(2*i);
      dict_set(D, k, v);
    }
    for (int i = 0; i < N; i++) {
      k = int_wrap(i);
      dict_get(D, k);
      memory_free(k);
    }
    for (int i = 0; i < N; i++) {
      k = int_wrap(i);
      I = dict_del(D, k);
      memory_free(k);
      item_total_destroy(I, memory_free, memory_free);
    }
    dict_destroy(D);

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
  test_dict_performance();

  return 0;
}
