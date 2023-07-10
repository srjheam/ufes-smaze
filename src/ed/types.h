
#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdbool.h>

typedef void (*destructor_fn)(void *);
typedef void *(*copy_fn)(void *);
typedef int (*cmp_fn)(void *, void *);
typedef size_t (*hash_fn)(void *);

typedef unsigned char byte;

#endif