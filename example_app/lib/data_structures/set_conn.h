#ifndef SET_CONN_H
#define SET_CONN_H

#include "utils.h"
#include "list.h"
#include "set.h"

struct _impl_set_conn_t;
typedef struct _impl_set_conn_t set_conn_t;

set_conn_t *set_conn_create(set_t *S);
void set_conn_destroy(set_conn_t *SC);

// Thread-safe read functions
size_t set_conn_len(set_conn_t *SC);
bool set_conn_includes(set_conn_t *SC, addr_t e);
list_t *set_conn_to_list(set_conn_t *SC);

// Thread-safe write functions
void set_conn_add(set_conn_t *SC, addr_t e);
addr_t set_conn_remove(set_conn_t *SC, addr_t e);

#endif
