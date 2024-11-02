#ifndef DICT_CONN_H
#define DICT_CONN_H

#include "utils.h"
#include "item.h"
#include "list.h"
#include "dict.h"

struct _impl_dict_conn_t;
typedef struct _impl_dict_conn_t dict_conn_t;

dict_conn_t *dict_conn_create(dict_t *D);
void dict_conn_destroy(dict_conn_t *DC);

// Thread-safe read functions
size_t dict_conn_len(dict_conn_t *DC);
addr_t dict_conn_get(dict_conn_t *DC, addr_t k);
list_t *dict_conn_items(dict_conn_t *DC);

// Thread-safe write functions
void dict_conn_set(dict_conn_t *DC, addr_t k, addr_t v);
item_t *dict_conn_del(dict_conn_t *DC, addr_t k);

#endif
