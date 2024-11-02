#include <assert.h>

#include "../include/list.h"
#include "../include/memory.h"
#include "../include/list_extended.h"

void list_total_destroy(list_t *L, void (*entry_destroy)(addr_t e)) {
  assert(L);

  addr_t e;
  for (int i = 0; i < list_len(L); i++) {
    e = list_get(L, i);
    if (e != NULL) {
      entry_destroy(e);
    }
  }
  list_destroy(L);
}

str_t list_string(list_t *L, str_t (*entry_string)(addr_t e)) {
  assert(L);

  addr_t e;
  str_t e_str;
  size_t len;

  size_t total_size = 0;
  for (int i = 0; i < list_len(L); i++) {
    e = list_get(L, i);
    e_str = entry_string(e);
    len = strlen(e_str);
    memory_free(e_str);
    total_size += len + 1; // +1 for comma
  }

  str_t res = (str_t) memory_calloc(total_size + 2 + 1, sizeof(char)); // +2 for brackets, +1 for \0
  strncat(res, "[", 1);

  for (int i = 0; i < list_len(L); i++) {
    e = list_get(L, i);
    e_str = entry_string(e);
    len = strlen(e_str);
    if (i != 0) {
      strncat(res, ",", 1);
    }
    strncat(res, e_str, len);
    memory_free(e_str);
  }

  strncat(res, "]", 1);

  return res;
}

list_t *list_copy(list_t *L) {
  assert(L);

  size_t n = list_len(L);
  list_t *C = list_create(n);
  addr_t e;
  for (int i = 0; i < n; i++) {
    e = list_get(L, i);
    list_set(C, i, e);
  }
  return C;
}

list_t *list_deep_copy(list_t *L, addr_t (*deep_copy)(addr_t e)) {
  assert(L);

  size_t n = list_len(L);
  list_t *C = list_create(n);
  addr_t e;
  addr_t c;
  for (int i = 0; i < n; i++) {
    e = list_get(L, i);
    c = deep_copy(e);
    list_set(C, i, c);
  }
  return C;
}

list_t *list_splice(list_t *L, size_t start, size_t end) {
  assert(L);

  assert(start < list_len(L));
  assert(end < list_len(L));
  assert(start <= end);
  
  list_t *S = list_create(end - start);
  addr_t e;
  for (int i = 0; i < end - start; i++) {
    e = list_get(L, start + i);
    list_set(S, i, e);
  }
  return S;
}

void _swap(list_t *L, size_t i, size_t j) {
  addr_t e1 = list_get(L, i);
  addr_t e2 = list_get(L, j);
  list_set(L, j, e1);
  list_set(L, i, e2);
}

void _merge(
    list_t *L,
    list_t *W,
    size_t start,
    size_t pivot,
    size_t end,
    int (*compare)(addr_t e1, addr_t e2)
) {
  size_t first_part_index = start;
  size_t second_part_index = pivot;
  size_t final_index = start;
  addr_t e1 = list_get(W, first_part_index);
  addr_t e2 = list_get(W, second_part_index);
  int comp;
  while (first_part_index != pivot || second_part_index != end) {
    comp = compare(e1, e2);
    if (first_part_index < pivot && (comp > 0 || second_part_index == end)) {
      list_set(L, final_index, e1);
      first_part_index++;
      final_index++;
      if (first_part_index < pivot) {
        e1 = list_get(W, first_part_index);
      }
    } else if (second_part_index < end) {
      // first_part_index == pivot or comp <= 0
      list_set(L, final_index, e2);
      second_part_index++;
      final_index++;
      if (second_part_index < end) {
        e2 = list_get(W, second_part_index);
      }
    }
  }
}

void _sort(
    list_t *L,
    list_t *W,
    size_t start,
    size_t end,
    int (*compare)(addr_t e1, addr_t e2)
) {
  if (end - start <= 1) {
    return;
  }
  size_t pivot = (end - start) / 2 + start;

  _sort(W, L, start, pivot, compare);
  _sort(W, L, pivot, end, compare);
  _merge(L, W, start, pivot, end, compare);
}

list_t *list_sort(list_t *L, int (*compare)(addr_t e1, addr_t e2)) {
  assert(L);

  list_t *S = list_copy(L);
  list_t *W = list_copy(S);
  _sort(S, W, 0, list_len(S), compare);
  list_destroy(W);
  return S;
}

list_t *list_unique(list_t *L, int (*compare)(addr_t e1, addr_t e2)) {
  assert(L);

  list_t *S = list_sort(L, compare);
  list_t *U = list_create(0);

  addr_t prev = NULL;
  addr_t curr = NULL;
  for (int i = 0; i < list_len(S); i++) {
    curr = list_get(S, i);
    if (prev == NULL || compare(prev, curr) != 0) {
      list_push(U, curr);
      prev = curr;
    }
  }

  list_destroy(S);

  return U;
}

list_t *list_concat(list_t *L1, list_t *L2) {
  assert(L1);
  assert(L2);

  list_t *L = list_copy(L1);
  addr_t e;
  for (int i = 0; i < list_len(L2); i++) {
    e = list_get(L2, i);
    list_push(L, e);
  }
  return L;
}

list_t *list_map(list_t *L, addr_t (*map)(addr_t e)) {
  assert(L);

  size_t n = list_len(L);
  list_t *M = list_create(n);
  addr_t e;
  for (int i = 0; i < n; i++) {
    e = list_get(L, i);
    list_set(M, i, map(e));
  }
  return M;
}

void list_for_each(list_t *L, void (*action)(addr_t e)) {
  assert(L);

  addr_t e;
  for (int i = 0; i < list_len(L); i++) {
    e = list_get(L, i);
    action(e);
  }
}

void list_reduce(list_t *L, void (*combine)(addr_t acc, addr_t e), addr_t acc) {
  assert(L);

  addr_t e;
  for (int i = 0; i < list_len(L); i++) {
    e = list_get(L, i);
    combine(acc, e);
  }
}
