
#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdlib.h>

#include "types.h"

typedef struct Heap Heap;

//
// I know that I could make it totally generic, by using an undetermined type
// for the priorities instead of using straight forward doubles, but I'm lazy.
//
Heap *heap_construct(bool ascending, size_t smemb, destructor_fn destructor);

void heap_push(Heap *heap, void *data, double priority);

bool heap_empty(Heap *heap);

void *heap_peek(Heap *heap);

double heap_max_priority(Heap *heap);

void *heap_pop(Heap *heap);

void heap_destroy(Heap *heap);

#endif