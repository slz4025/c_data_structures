#include <assert.h>

#include "../include/item.h"
#include "../include/list.h"
#include "../include/memory.h"
#include "../include/dict.h"
#include "../include/dict_extended.h"

void dict_total_destroy(dict_t *D, void (*key_destroy)(addr_t k), void (*value_destroy)(addr_t v)) {
  assert(D);

  list_t *items = dict_items(D);

  item_t *I;
  addr_t k;
  addr_t v;

  for (int i = 0; i < list_len(items); i++) {
    I = list_get(items, i);
    k = item_get_key(I);
    v = item_get_value(I);
    key_destroy(k);
    value_destroy(v);
  }

  list_destroy(items);
  dict_destroy(D);
}

str_t dict_string(dict_t *D, str_t (*key_string)(addr_t k), str_t (*value_string)(addr_t v)) {
  assert(D);

  list_t *items = dict_items(D);

  item_t *I;
  addr_t k;
  addr_t v;
  str_t k_str;
  str_t v_str;
  size_t k_len;
  size_t v_len;

  size_t total_size = 0;
  for (int i = 0; i < list_len(items); i++) {
    I = list_get(items, i);
    k = item_get_key(I);
    v = item_get_value(I);
    k_str = key_string(k);
    v_str = value_string(v);
    k_len = strlen(k_str);
    v_len = strlen(v_str);
    memory_free(k_str);
    memory_free(v_str);
    total_size += k_len + 1 + v_len + 1; // +1 for colon, +1 for comma
  }

  str_t res = (str_t) memory_calloc(total_size + 2 + 1, sizeof(char)); // +2 for brackets, +1 for \0
  strncat(res, "{", 1);

  for (int i = 0; i < list_len(items); i++) {
    I = list_get(items, i);
    k = item_get_key(I);
    v = item_get_value(I);
    k_str = key_string(k);
    v_str = value_string(v);
    k_len = strlen(k_str);
    v_len = strlen(v_str);
    if (i != 0) {
      strncat(res, ",", 1);
    }
    strncat(res, k_str, k_len);
    strncat(res, ":", 1);
    strncat(res, v_str, v_len);
    memory_free(k_str);
    memory_free(v_str);
  }

  strncat(res, "}", 1);

  list_destroy(items);

  return res;
}

addr_t _item_get_key(addr_t e) {
  return item_get_key((item_t *) e);
}

list_t *dict_keys(dict_t *D) {
  assert(D);

  list_t *items = dict_items(D);

  item_t *I;
  addr_t k;

  list_t *keys = list_create(0);
  for (int i = 0; i < list_len(items); i++) {
    I = list_get(items, i);
    k = _item_get_key(I);
    list_push(keys, k);
  }

  list_destroy(items);
  return keys;
}

addr_t _item_get_value(addr_t e) {
  return item_get_value((item_t *) e);
}

list_t *dict_values(dict_t *D) {
  assert(D);

  list_t *items = dict_items(D);

  item_t *I;
  addr_t v;

  list_t *values = list_create(0);
  for (int i = 0; i < list_len(items); i++) {
    I = list_get(items, i);
    v = _item_get_value(I);
    list_push(values, v);
  }

  list_destroy(items);
  return values;
}

dict_t *dict_copy(dict_t *D) {
  assert(D);

  dict_t *C = dict_create(dict_key_eq(D), dict_key_hash(D));

  item_t *I;
  addr_t k;
  addr_t v;

  list_t *items = dict_items(D);
  for (int i = 0; i < list_len(items); i++) {
    I = list_get(items, i);
    k = item_get_key(I);
    v = item_get_value(I);
    dict_set(C, k, v);
  }
  list_destroy(items);

  return C;
}

dict_t *dict_deep_copy(dict_t *D, addr_t (*key_copy)(addr_t k), addr_t (*value_copy)(addr_t v)) {
  assert(D);

  dict_t *C = dict_create(dict_key_eq(D), dict_key_hash(D));

  item_t *I;
  addr_t k;
  addr_t v;
  addr_t k_copy;
  addr_t v_copy;

  list_t *items = dict_items(D);
  for (int i = 0; i < list_len(items); i++) {
    I = list_get(items, i);
    k = item_get_key(I);
    k_copy = key_copy(k);
    v = item_get_value(I);
    v_copy = value_copy(v);
    dict_set(C, k_copy, v_copy);
  }
  list_destroy(items);

  return C;
}
