#include <assert.h>

#include "../include/item.h"
#include "../include/list.h"
#include "../include/memory.h"
#include "../include/dict.h"

/* List implementation of the bucket.
 * If the size of each bucket is small,
 * then a list_t is simpler to use
 * and practically more efficient,
 * a heap, despite the latter being
 * theoretically more efficient.
 */
struct _bucket {
  list_t *L;
  bool (*key_eq) (addr_t k1, addr_t k2);
};
typedef struct _bucket bucket;

bucket *bucket_create(bool (*key_eq) (addr_t k1, addr_t k2)) {
  list_t *L = list_create(0);
  bucket *B = (bucket *) memory_malloc(sizeof(bucket));
  B->L = L;
  B->key_eq = key_eq;
  return B;
}

void bucket_destroy(bucket *B) {
  assert(B);

  size_t n = list_len(B->L);

  addr_t I;
  for (int i = 0; i < n; i++) {
    I = list_get(B->L, i); 
    item_destroy(I);
  }

  list_destroy(B->L);
  memory_free(B);
}

item_t *_bucket_item(bucket *B, addr_t k) {
  assert(B);

  size_t n = list_len(B->L);

  item_t *I;
  for (int i = 0; i < n; i++) {
    I = (item_t *) list_get(B->L, i);
    if (B->key_eq(item_get_key(I), k)) {
      return I;
    }
  }
  return NULL;
}

addr_t bucket_get(bucket *B, addr_t k) {
  assert(B);

  item_t *I = _bucket_item(B, k);
  if (I == NULL) {
    return NULL;
  } else {
    return item_get_value(I);
  }
}

void bucket_set(bucket *B, addr_t k, addr_t v) {
  assert(B);

  item_t *I = _bucket_item(B, k);
  if (I == NULL) {
    I = item_create(k, v);
    list_push(B->L, I);
  } else {
    item_set_value(I, v);
  }
}

item_t *bucket_del(bucket *B, addr_t k) {
  assert(B);

  size_t n = list_len(B->L);

  addr_t I;
  for (int i = 0; i < n; i++) {
    I = list_get(B->L, i);
    if (B->key_eq(item_get_key(I), k)) {
      list_remove(B->L, i);
      return I;
    }
  }

  return NULL;
}

list_t *bucket_items(bucket *B) {
  assert(B);

  return B->L;
}

struct _impl_dict_t {
  /* Allow amortized O(1) get/set of a value at a key.
   *
   * Use a list_t of size m, with a bucket type in each position.
   * Map dict_t keys -> list_t indices using a hash function.
   * Then, search for the key in the bucket.
   *
   * Always ensure m >= C * n, C being a constant;
   * then in expectation, there are <=C keys at each index.
   * Then can do O(n) resize operation whenver the following is not true:
   * m / 2 < n <= m.
   */
  size_t len;
  list_t *buckets;
  size_t (*key_hash) (addr_t k);
  bool (*key_eq) (addr_t k1, addr_t k2);
};

size_t _buckets_index(list_t *buckets, addr_t k, size_t (*key_hash)(addr_t k)) {
  size_t num_buckets = list_len(buckets);
  assert(num_buckets > 0);

  size_t index;
  if (num_buckets == 1) {
    index = 0;
  } else {
    size_t hash = key_hash(k);
    index = hash % num_buckets;
  }
  return index;
}

bucket *_get_bucket(list_t *buckets, addr_t k, size_t (*key_hash)(addr_t k)) {
  size_t num_buckets = list_len(buckets);
  if (num_buckets == 0) {
    return NULL;
  }
  size_t index = _buckets_index(buckets, k, key_hash);
  assert(index < num_buckets);
  bucket *B = (bucket *) list_get(buckets, index);
  return B;
} 

void _dict_resize(dict_t *D, size_t new_len) {
  size_t curr_capacity = list_len(D->buckets);
  if (curr_capacity / 2 < new_len && new_len <= curr_capacity) {
    D->len = new_len;
    return;
  }

  size_t new_capacity = 0;
  if (new_len > 0) {
    if (curr_capacity == 0) {
      new_capacity = 1;
    } else {
      new_capacity = curr_capacity;
    }
    while (new_capacity < new_len) {
      new_capacity *= 2;
    } 
    while (new_capacity >= 2 * new_len) {
      new_capacity /= 2;
    }
  }

  bucket *B;
  addr_t I;
  addr_t k;
  addr_t v;

  list_t *buckets = list_create(new_capacity);
  for (int i = 0; i < new_capacity; i++) {
    B = bucket_create(D->key_eq);
    list_set(buckets, i, B);
  }

  list_t *items = dict_items(D);
  size_t copy_len = list_len(items);
  if (new_capacity < copy_len) {
    copy_len = new_capacity;
  }
  for (int i = 0; i < copy_len; i++) {
    I = list_get(items, i); 
    k = item_get_key(I);
    v = item_get_value(I);
    B = _get_bucket(buckets, k, D->key_hash);
    assert(B);
    bucket_set(B, k, v);
  }
  list_destroy(items);

  list_t *old_buckets = D->buckets;
  for (int i = 0; i < list_len(old_buckets); i++) {
    B = list_get(old_buckets, i);
    bucket_destroy(B);
  }
  list_destroy(old_buckets);

  D->len = new_len;
  D->buckets = buckets;
}

dict_t *dict_create(bool (*key_eq) (addr_t k1, addr_t k2), size_t (*key_hash) (addr_t k)) {
  dict_t *D = (dict_t *) memory_malloc(sizeof(dict_t));

  D->len = 0;
  D->buckets = list_create(0);
  D->key_hash = key_hash;
  D->key_eq = key_eq;

  return D;
}

void dict_destroy(dict_t *D) {
  assert(D);

  bucket *B;
  for (int i = 0; i < list_len(D->buckets); i++) {
    B = list_get(D->buckets, i);
    bucket_destroy(B);
  }
  list_destroy(D->buckets);

  memory_free(D);
}

addr_t dict_key_eq(dict_t *D) {
  assert(D);

  return D->key_eq;
}

addr_t dict_key_hash(dict_t *D) {
  assert(D);

  return D->key_hash;
}

size_t dict_len(dict_t *D) {
  assert(D);

  size_t len = D->len;
  return len;
}

addr_t dict_get(dict_t *D, addr_t k) {
  assert(D);

  bucket *B = _get_bucket(D->buckets, k, D->key_hash);
  if (B == NULL) {
    return NULL;
  }
  addr_t v = bucket_get(B, k);
  return v;
}

list_t *dict_items(dict_t *D) {
  assert(D);

  list_t *all_items = list_create(0);
  size_t num_buckets = list_len(D->buckets);

  list_t *items;
  bucket *B;
  item_t *I;
  for (int i = 0; i < num_buckets; i++) {
    B = (bucket *) list_get(D->buckets, i);
    items = bucket_items(B);
    for (int j = 0; j < list_len(items); j++) {
      I = list_get(items, j);
      list_push(all_items, I);
    };
  }
  return all_items;
}

void dict_set(dict_t *D, addr_t k, addr_t v) {
  assert(D);

  if (dict_get(D, k) == NULL) {
    _dict_resize(D, D->len + 1);
  }
  bucket *B = _get_bucket(D->buckets, k, D->key_hash);
  assert(B);
  bucket_set(B, k, v);
}

item_t *dict_del(dict_t *D, addr_t k) {
  assert(D);

  if (dict_get(D, k) == NULL) {
    return NULL;
  }

  bucket *B = _get_bucket(D->buckets, k, D->key_hash);
  assert(B);

  item_t *I = bucket_del(B, k);
  _dict_resize(D, D->len - 1);
  return I;
}
