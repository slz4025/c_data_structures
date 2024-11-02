#include <assert.h>

#include "../include/item.h"
#include "../include/list_extended.h"
#include "../include/memory.h"
#include "../include/set.h"

void set_total_destroy(set_t *S, void (*entry_destroy)(addr_t e)) {
  assert(S);

  list_t *entries = set_to_list(S);
  list_total_destroy(entries, entry_destroy);
  set_destroy(S);
}

str_t set_string(set_t *S, str_t (*entry_string)(addr_t e)) {
  assert(S);

  list_t *entries = set_to_list(S);

  addr_t e;
  str_t e_str;
  size_t e_len;

  size_t total_size = 0;
  for (int i = 0; i < list_len(entries); i++) {
    e = list_get(entries, i);
    e_str = entry_string(e);
    e_len = strlen(e_str);
    memory_free(e_str);
    total_size += e_len + 1; // +1 for comma
  }

  str_t res = (str_t) memory_calloc(total_size + 2 + 1, sizeof(char)); // +2 for brackets, +1 for \0
  strncat(res, "{", 1);

  for (int i = 0; i < list_len(entries); i++) {
    e = list_get(entries, i);
    e_str = entry_string(e);
    e_len = strlen(e_str);
    if (i != 0) {
      strncat(res, ",", 1);
    }
    strncat(res, e_str, e_len);
    memory_free(e_str);
  }

  strncat(res, "}", 1);

  list_destroy(entries);

  return res;
}

set_t *set_union(set_t *S1, set_t *S2, bool (*entry_eq) (addr_t e1, addr_t e2), size_t (*hash) (addr_t e)) {
  assert(S1);
  assert(S2);

  set_t *S = set_create(entry_eq, hash);

  addr_t e;

  list_t *L1 = set_to_list(S1);
  for (int i = 0; i < list_len(L1); i++) {
    e = list_get(L1, i);
    set_add(S, e);
  }
  list_destroy(L1);

  list_t *L2 = set_to_list(S2);
  for (int i = 0; i < list_len(L2); i++) {
    e = list_get(L2, i);
    set_add(S, e);
  }
  list_destroy(L2);

  return S;
}

set_t *set_intersection(set_t *S1, set_t *S2, bool (*entry_eq) (addr_t e1, addr_t e2), size_t (*hash) (addr_t e)) {
  assert(S1);
  assert(S2);

  set_t *S = set_create(entry_eq, hash);

  addr_t e;

  list_t *L1 = set_to_list(S1);
  for (int i = 0; i < list_len(L1); i++) {
    e = list_get(L1, i);
    if (set_includes(S2, e)) {
      set_add(S, e);
    }
  }
  list_destroy(L1);

  return S;
}

set_t *set_difference(set_t *S1, set_t *S2, bool (*entry_eq) (addr_t e1, addr_t e2), size_t (*hash) (addr_t e)) {
  assert(S1);
  assert(S2);

  set_t *S = set_create(entry_eq, hash);

  addr_t e;

  list_t *L1 = set_to_list(S1);
  for (int i = 0; i < list_len(L1); i++) {
    e = list_get(L1, i);
    if (!set_includes(S2, e)) {
      set_add(S, e);
    }
  }
  list_destroy(L1);

  return S;
}

set_t *set_from_list(list_t *L, bool (*entry_eq) (addr_t e1, addr_t e2), size_t (*hash) (addr_t e)) {
  assert(L);

  set_t *S = set_create(entry_eq, hash);

  addr_t e;
  for (int i = 0; i < list_len(L); i++) {
    e = list_get(L, i);
    set_add(S, e);
  }

  return S;
}

set_t *set_copy(set_t *S) {
  assert(S);

  list_t *entries = set_to_list(S);
  set_t *C = set_from_list(entries, set_key_eq(S), set_hash(S));
  list_destroy(entries);

  return C;
}

set_t *set_deep_copy(set_t *S, addr_t (*entry_copy)(addr_t e)) {
  assert(S);

  list_t *entries = set_to_list(S);
  list_t *entries_copied = list_deep_copy(entries, entry_copy);
  set_t *C = set_from_list(entries_copied, set_key_eq(S), set_hash(S));
  list_destroy(entries_copied);
  list_destroy(entries);

  return C;
}
