#include <string.h>

#include "heap.h"

#define _HEAP_SIZINI 4

void __heap_grow(Heap *heap) {
    if (heap->len == heap->capacity) {
        heap->capacity <<= 1;
        heap->data = realloc(heap->data, heap->smemb * heap->capacity);
        heap->priorities =
            realloc(heap->priorities, sizeof(double) * heap->capacity);
    }
}

// Maybe the heap should be implemented as a segmented array, like the deque.
struct Heap {
    void *data;
    double *priorities;
    size_t capacity;
    size_t len;

    bool ascending;
    size_t smemb;
    copy_fn copy;
    destructor_fn destructor;
};

Heap *heap_construct(bool ascending, size_t smemb, copy_fn copy, destructor_fn destructor) {
    Heap *heap = malloc(sizeof *heap);

    heap->ascending = ascending;
    heap->smemb = smemb;
    heap->copy = copy;
    heap->destructor = destructor;

    heap->capacity = _HEAP_SIZINI;
    heap->data = malloc(heap->smemb * heap->capacity);
    heap->priorities = malloc(sizeof(double) * heap->capacity);
    heap->len = 0;

    return heap;
}

void heap_push(Heap *heap, void *data, double priority) {
    __heap_grow(heap);

    heap->priorities[heap->len] = priority;
    memcpy(heap->data + heap->smemb * heap->len, data, heap->smemb);
    heap->len++;
    
    // heapify
}

bool heap_empty(Heap *heap) { return heap->len == 0; }

void *heap_max(Heap *heap) { return heap->data; }

double heap_max_priority(Heap *heap) { return heap->priorities[0]; }

void *heap_pop(Heap *heap) {
    void *data = heap->copy(heap->data);

    if (heap->destructor != NULL)
        heap->destructor(heap->data);

    memmove(heap->data, heap->data + heap->smemb, heap->smemb * --heap->len);

    return data;
}

void heap_destroy(Heap *heap) {
    if (heap->destructor != NULL)
        for (size_t i = 0; i < heap->len; i++)
            heap->destructor(heap->data + heap->smemb * i);

    free(heap->data);
    free(heap->priorities);
    free(heap);
}
