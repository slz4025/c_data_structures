#ifndef DICT_EXTENDED_H
#define DICT_EXTENDED_H

#include "utils.h"
#include "item.h"
#include "list.h"
#include "dict.h"

void dict_total_destroy(dict_t *D, void (*key_destroy)(addr_t k), void (*value_destroy)(addr_t v));
str_t dict_string(dict_t *D, str_t (*key_string)(addr_t k), str_t (*value_string)(addr_t v));

dict_t *dict_copy(dict_t *D);
dict_t *dict_deep_copy(dict_t *D, addr_t (*key_copy)(addr_t k), addr_t (*value_copy)(addr_t v));

list_t *dict_keys(dict_t *D);
list_t *dict_values(dict_t *D);

#endif
