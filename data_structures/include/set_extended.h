#ifndef SET_EXTENDED_H
#define SET_EXTENDED_H

#include "set.h"

void set_total_destroy(set_t *S, void (*entry_destroy)(addr_t e));
str_t set_string(set_t *S, str_t (*entry_string)(addr_t e));

set_t *set_from_list(list_t *L, bool (*entry_eq) (addr_t e1, addr_t e2), size_t (*hash) (addr_t e));

set_t *set_copy(set_t *S);
set_t *set_deep_copy(set_t *S, addr_t (*entry_copy)(addr_t e));

set_t *set_union(set_t *S1, set_t *S2, bool (*entry_eq) (addr_t e1, addr_t e2), size_t (*hash) (addr_t e));
set_t *set_intersection(set_t *S1, set_t *S2, bool (*entry_eq) (addr_t e1, addr_t e2), size_t (*hash) (addr_t e));
set_t *set_difference(set_t *S1, set_t *S2, bool (*entry_eq) (addr_t e1, addr_t e2), size_t (*hash) (addr_t e));

#endif
