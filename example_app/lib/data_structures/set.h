#ifndef SET_H
#define SET_H

#include "utils.h"
#include "list.h"

struct _impl_set_t;
typedef struct _impl_set_t set_t;

set_t *set_create(bool (*entry_eq) (addr_t e1, addr_t e2), size_t (*hash) (addr_t e));
void set_destroy(set_t *S);

addr_t set_key_eq(set_t *S);
addr_t set_hash(set_t *S);
size_t set_len(set_t *S);
bool set_includes(set_t *S, addr_t e);
list_t *set_to_list(set_t *S);

void set_add(set_t *S, addr_t e);
addr_t set_remove(set_t *S, addr_t e);

#endif
