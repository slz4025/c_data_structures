#ifndef MEMORY_H
#define MEMORY_H

#include "utils.h"

void memory_pointers_init();
str_t memory_pointers_report();
void memory_pointers_finish();

void memory_count_reset();
size_t memory_count_report();

addr_t memory_malloc(size_t num_bytes);
addr_t memory_calloc(size_t num_entries, size_t num_bytes);
void memory_free(addr_t p);

#endif
