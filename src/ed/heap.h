
#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdlib.h>

#include "types.h"

typedef struct Heap Heap;

Heap *heap_construct(bool ascending, size_t smemb, copy_fn copy, destructor_fn destructor);

void heap_push(Heap *heap, void *data, double priority);

bool heap_empty(Heap *heap);

void *heap_peek(Heap *heap);

double heap_max_priority(Heap *heap);

void *heap_pop(Heap *heap);

void heap_destroy(Heap *heap);

#endif