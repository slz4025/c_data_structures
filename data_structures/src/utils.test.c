#include <assert.h>
#include <stdio.h>

#include "../include/utils.h"

void test_lu_log_2() {
  printf("unsigned long log 2\n");
  assert(lu_log_2(1) == 0);
  assert(lu_log_2(2) == 1);
  assert(lu_log_2(3) == 1);
  assert(lu_log_2(4) == 2);
  assert(lu_log_2(5) == 2);
  assert(lu_log_2(6) == 2);
  assert(lu_log_2(7) == 2);
  assert(lu_log_2(8) == 3);
  assert(lu_log_2(16) == 4);
  assert(lu_log_2(32) == 5);
  assert(lu_log_2(64) == 6);
  assert(lu_log_2(128) == 7);
  assert(lu_log_2(256) == 8);
  assert(lu_log_2(512) == 9);
  assert(lu_log_2(1024) == 10);
}

int main() {
  test_lu_log_2();

  return 0;
}
