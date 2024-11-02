#ifndef UTILS_H
#define UTILS_H

/* For a list of standard libraries in C, refer to:
 * https://en.wikibooks.org/wiki/C_Programming/Standard_libraries
 */
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef void *addr_t;
typedef char *str_t;

extern size_t INT_MAX_STR_SIZE;
extern size_t LU_MAX_STR_SIZE;
extern size_t PTR_MAX_STR_SIZE;

// Floored representation of the log of X in base 2.
// Do not input X = 0 as the value is undefined.
size_t lu_log_2(size_t X);

#endif
