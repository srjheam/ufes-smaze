#include <string.h>

#include "heap.h"

#define _HEAP_SIZINI 4

// Maybe the heap should be implemented as a segmented array, like the deque.
struct Heap {
    void *data;
    double *priorities;
    size_t capacity;
    size_t len;

    bool ascending;
    size_t smemb;
    destructor_fn destructor;
};

void __heap_grow(Heap *heap) {
    if (heap->len == heap->capacity) {
        heap->capacity <<= 1;
        heap->data = realloc(heap->data, heap->smemb * heap->capacity);
        heap->priorities =
            realloc(heap->priorities, sizeof(double) * heap->capacity);
    }
}

double __heap_cmp(Heap *heap, double a, double b) {
    return heap->ascending ? b - a : a - b;
}

void __heap_swap(Heap *heap, size_t i, size_t j) {
    size_t lindx = 2 * i + 1;
    size_t rindx = 2 * i + 2;

    double pp = heap->priorities[i];
    double pl = lindx < heap->len ? heap->priorities[lindx] : 0;
    double pr = rindx < heap->len ? heap->priorities[rindx] : 0;

    void *tmp = malloc(heap->smemb);
    memcpy(tmp, heap->data + i * heap->smemb, heap->smemb);
    memcpy(heap->data + i * heap->smemb, heap->data + j * heap->smemb,
           heap->smemb);
    free(tmp);

    heap->priorities[i] =
        (lindx < heap->len && __heap_cmp(heap, pp, pl) < 0) ? pl : pr;
    heap->priorities[j] = pp;
}

double __heap_heapify_high(Heap *heap, size_t i) {
    size_t lindx = 2 * i + 1;
    size_t rindx = 2 * i + 2;

    double pp = heap->priorities[i];
    double pl = lindx < heap->len ? heap->priorities[lindx] : 0;
    double pr = rindx < heap->len ? heap->priorities[rindx] : 0;

    double ldiff = lindx < heap->len ? __heap_cmp(heap, pp, pl) : 0;
    double rdiff = rindx < heap->len ? __heap_cmp(heap, pp, pr) : 0;

    return ldiff > rdiff ? ldiff * -1 : rdiff;
}

void __heap_heapify_up(Heap *heap) {
    if (heap->len == 1)
        return;

    size_t icurr = (heap->len >> 1) - 1;
    while (__heap_heapify_high(heap, icurr)) {
        size_t ichild =
            2 * icurr + __heap_heapify_high(heap, icurr) < 0 ? 1 : 2;

        __heap_swap(heap, icurr, ichild);

        icurr = ((icurr + 1) >> 1) - 1;
    }
}

void __heap_heapify_down(Heap *heap) {
    size_t icurr = 0;
    while (__heap_heapify_high(heap, icurr)) {
        size_t ichild =
            2 * icurr + __heap_heapify_high(heap, icurr) < 0 ? 1 : 2;

        __heap_swap(heap, icurr, ichild);

        icurr = ichild;
    }
}

Heap *heap_construct(bool ascending, size_t smemb, destructor_fn destructor) {
    Heap *heap = malloc(sizeof *heap);

    heap->ascending = ascending;
    heap->smemb = smemb;
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
    memcpy(heap->data + heap->smemb * heap->len++, data, heap->smemb);

    __heap_heapify_up(heap);
}

bool heap_empty(Heap *heap) { return heap->len == 0; }

void *heap_peek(Heap *heap) { return heap->data; }

double heap_max_priority(Heap *heap) { return heap->priorities[0]; }

void *heap_pop(Heap *heap) {
    void *data = malloc(heap->smemb);
    memcpy(data, heap->data, heap->smemb);

    if (heap->destructor != NULL)
        heap->destructor(heap->data);

    memcpy(heap->data, heap->data + heap->smemb * --heap->len, heap->smemb);
    heap->priorities[0] = heap->priorities[heap->len];

    __heap_heapify_down(heap);

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
