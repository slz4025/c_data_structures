#include <assert.h>

#include "../include/utils.h"

size_t INT_MAX_STR_SIZE = 10;
size_t LU_MAX_STR_SIZE = 10;
size_t PTR_MAX_STR_SIZE = 20;

size_t lu_log_2(size_t X) {
  assert(X != 0); // undefined case
  size_t log = 0;
  while (X > 1) {
    X >>= 1;
    log += 1;
  }
  return log;
}
