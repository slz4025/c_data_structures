#include <assert.h>
#include <ctype.h>
#include <stdio.h>

#include "../include/str.h"
#include "../include/memory.h"

addr_t str_wrap(str_t s) {
  addr_t e = memory_malloc(sizeof(str_t));
  *((str_t *) e) = s;
  return e;
}

str_t str_unwrap(addr_t e) {
  assert(e);

  str_t s = *((str_t *) e);
  return s;
}

str_t str_splice(str_t s, size_t start, size_t end) {
  assert(s);

  str_t splice = (str_t) memory_calloc(end - start + 1, sizeof(char));
  strncpy(splice, s + start, end - start);
  return splice;
}

str_t str_join(list_t *L, str_t delimiter) {
  assert(L);

  str_t token;

  size_t total_len = 0;
  for (int i = 0; i < list_len(L); i++) {
    if (i > 0) {
      total_len += strlen(delimiter);
    }
    token = str_unwrap(list_get(L, i));
    total_len += strlen(token);
  }

  str_t acc = (str_t) memory_calloc(total_len + 1, sizeof(char));
  for (int i = 0; i < list_len(L); i++) {
    if (i > 0) {
      acc = strncat(acc, delimiter, strlen(delimiter));
    }
    token = str_unwrap(list_get(L, i));
    acc = strncat(acc, token, strlen(token));
  }

  return acc;
}

list_t *str_split(str_t s, str_t delimiter) {
  assert(s);

  size_t n = strlen(s);
  size_t dn = strlen(delimiter);

  list_t *L = list_create(0);

  // edge-case: return string when encounter "" delimiter
  // unless string is "", in which case return empty list
  if (dn == 0) {
    if (n > 0) {
      str_t c = (str_t) memory_calloc(n, sizeof(char));
      strlcpy(c, s, n+1); // +1 for \0
      list_push(L, str_wrap(c));
    }
    return L;
  }

  addr_t token;
  char *prev = s;
  char *curr = s;
  size_t prev_index = 0;
  size_t curr_index = 0;
  while (curr_index < n) {
    curr = strstr(prev, delimiter);
    if (curr == NULL) {
      curr_index = n;
    } else {
      curr_index = (curr - prev) + prev_index;
    }

    token = str_wrap(str_splice(s, prev_index, curr_index));
    list_push(L, token);

    prev_index = curr_index + dn;
    prev = curr + dn;
  }

  return L;
}

list_t *str_splitlines(str_t s) {
  assert(s);

  return str_split(s, "\n");
}

str_t str_replace(str_t s, str_t sub1, str_t sub2) {
  assert(s);

  list_t *L = str_split(s, sub1);
  str_t ns = str_join(L, sub2);

  addr_t e;
  str_t t;
  for (int i = 0; i < list_len(L); i++) {
    e = list_get(L, i);
    t = str_unwrap(e);
    memory_free(t);
    memory_free(e);
  }
  list_destroy(L);

  return ns;
}

str_t str_strip(str_t s) {
  assert(s);

  size_t n = strlen(s);
  size_t start = 0;
  while (isspace(s[start])) {
    start++;
  }
  size_t end = n;
  while (end > start && isspace(s[end-1])) {
    end--;
  }

  str_t stripped = str_splice(s, start, end);
  return stripped;
}
