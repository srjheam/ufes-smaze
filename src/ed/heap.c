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

int __heap_cmp(Heap *heap, double a, double b) {
    return heap->ascending ? b - a : a - b;
}

void __heap_heapify_up(Heap *heap) {
    if (heap->len == 1)
        return;

    size_t icurr = (heap->len >> 1) - 1;
    size_t lindx = 2 * icurr + 1;
    size_t rindx = 2 * icurr + 2;

    double pp = heap->priorities[icurr];
    double pl = heap->priorities[lindx];
    double pr = heap->priorities[rindx];

    while (icurr > 0 || __heap_cmp(heap, pp, pl) < 0 ||
           __heap_cmp(heap, pp, pr) < 0) {
        if (__heap_cmp(heap, pp, pl) < 0 || __heap_cmp(heap, pp, pr)) {
            size_t ichild = __heap_cmp(heap, pp, pl) < 0 ? lindx : rindx;

            void *tmp = malloc(heap->smemb);
            memcpy(tmp, heap->data + icurr * heap->smemb, heap->smemb);
            memcpy(heap->data + icurr * heap->smemb,
                   heap->data + ichild * heap->smemb, heap->smemb);
            free(tmp);

            heap->priorities[icurr] = __heap_cmp(heap, pp, pl) < 0 ? pl : pr;
            heap->priorities[ichild] = pp;
        } else
            break;

        icurr = ((icurr + 1) >> 1) - 1;
        lindx = 2 * icurr + 1;
        rindx = 2 * icurr + 2;

        pp = heap->priorities[icurr];
        pl = heap->priorities[lindx];
        pr = heap->priorities[rindx];
    }
}

void __heap_heapify_down(Heap *heap) {
    if (heap->len <= 1)
        return;

    size_t icurr = 0;
    size_t lindx = 2 * icurr + 1;
    size_t rindx = 2 * icurr + 2;

    double pp = heap->priorities[icurr];
    double pl = lindx < heap->len ? heap->priorities[lindx] : 0;
    double pr = lindx < heap->len ? heap->priorities[rindx] : 0;

    while (icurr != heap->len - 1 ||
           (lindx < heap->len && __heap_cmp(heap, pp, pl) < 0) ||
           (rindx < heap->len && __heap_cmp(heap, pp, pr) < 0)) {
        if ((lindx < heap->len && __heap_cmp(heap, pp, pl) < 0) ||
            (rindx < heap->len && __heap_cmp(heap, pp, pr))) {
            size_t ichild = (lindx < heap->len && __heap_cmp(heap, pp, pl)) < 0
                                ? lindx
                                : rindx;

            void *tmp = malloc(heap->smemb);
            memcpy(tmp, heap->data + icurr * heap->smemb, heap->smemb);
            memcpy(heap->data + icurr * heap->smemb,
                   heap->data + ichild * heap->smemb, heap->smemb);
            free(tmp);

            heap->priorities[icurr] =
                (lindx < heap->len && __heap_cmp(heap, pp, pl) < 0) ? pl : pr;
            heap->priorities[ichild] = pp;
        } else
            break;

        lindx = 2 * icurr + 1;
        rindx = 2 * icurr + 2;

        pp = heap->priorities[icurr];
        pl = heap->priorities[lindx];
        pr = heap->priorities[rindx];
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

Heap *heap_construct(bool ascending, size_t smemb, copy_fn copy,
                     destructor_fn destructor) {
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
    memcpy(heap->data + heap->smemb * heap->len++, data, heap->smemb);

    __heap_heapify_up(heap);
}

bool heap_empty(Heap *heap) { return heap->len == 0; }

void *heap_peek(Heap *heap) { return heap->data; }

double heap_max_priority(Heap *heap) { return heap->priorities[0]; }

void *heap_pop(Heap *heap) {
    void *data = heap->copy(heap->data);

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
