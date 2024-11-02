#ifndef STR_H
#define STR_H

#include "utils.h"
#include "list.h"

addr_t str_wrap(str_t s);
str_t str_unwrap(addr_t e);

str_t str_splice(str_t s, size_t start, size_t end);

str_t str_join(list_t *L, str_t delimiter);
list_t *str_split(str_t s, str_t delimiter);
list_t *str_splitlines(str_t s);

str_t str_replace(str_t s, str_t sub1, str_t sub2);
str_t str_strip(str_t s);

#endif
