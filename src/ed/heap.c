#include <string.h>

#include "exceptions.h"

#include "heap.h"

#define _HEAP_SIZINI 4

// Maybe the heap should be implemented as a segmented array, like the deque.
struct Heap {
    byte *data;
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
    if (i >= heap->len || j >= heap->len)
        exception_throw_index("__heap_swap - Index out of bounds");

    void *tmp = malloc(heap->smemb);
    memcpy(tmp, heap->data + i * heap->smemb, heap->smemb);
    memcpy(heap->data + i * heap->smemb, heap->data + j * heap->smemb,
           heap->smemb);
    memcpy(heap->data + j * heap->smemb, tmp, heap->smemb);
    free(tmp);

    double tmp2 = heap->priorities[i];
    heap->priorities[i] = heap->priorities[j];
    heap->priorities[j] = tmp2;
}

/**
 * @brief Returns the priority of the child with the highest priority; if the
 * result is negative, the left child has the highest priority; if the result is
 * positive, the right child has the highest priority; if the result is zero,
 * the children have less priority than the parent.
 *
 * @param heap The heap.
 * @param i The index of the parent.
 * @return double
 */
double __heap_heapify_high(Heap *heap, size_t i) {
    size_t lindx = 2 * i + 1;
    size_t rindx = 2 * i + 2;

    // I love Linux file permissions.
    int bits = 0b01 * (lindx >= heap->len) + 0b10 * (rindx >= heap->len);
    switch (bits) {
    case 0b01:
        return __heap_cmp(heap, heap->priorities[i], heap->priorities[rindx]) <
                       0
                   ? heap->priorities[rindx]
                   : 0;

    case 0b10:
        return __heap_cmp(heap, heap->priorities[i], heap->priorities[lindx]) <
                       0
                   ? heap->priorities[lindx] * -1
                   : 0;

    case 0b11:
        return 0;
    }

    double pp = heap->priorities[i];
    double pl = heap->priorities[lindx];
    double pr = heap->priorities[rindx];

    double ldiff = __heap_cmp(heap, pl, pp);
    double rdiff = __heap_cmp(heap, pr, pp);

    int bits2 = 0b01 * (ldiff <= 0) + 0b10 * (rdiff <= 0);
    switch (bits2) {
    case 0b01:
        return pr;

    case 0b10:
        return pl * -1;

    case 0b11:
        return 0;
    }

    return ldiff < rdiff ? pr : pl * -1;
}

void __heap_heapify_up(Heap *heap) {
    if (heap->len == 1)
        return;

    size_t icurr = (heap->len >> 1) - 1;
    while (__heap_heapify_high(heap, icurr)) {
        size_t ichild =
            2 * icurr + (__heap_heapify_high(heap, icurr) < 0 ? 1 : 2);

        __heap_swap(heap, icurr, ichild);

        if (icurr == 0)
            break;
        icurr = ((icurr + 1) >> 1) - 1;
    }
}

void __heap_heapify_down(Heap *heap) {
    size_t icurr = 0;
    while (__heap_heapify_high(heap, icurr)) {
        size_t ichild =
            2 * icurr + (__heap_heapify_high(heap, icurr) < 0 ? 1 : 2);

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

Kvp *heap_pop(Heap *heap) {
    double *priority = malloc(sizeof(double));
    *priority = heap->priorities[0];

    void *data = malloc(heap->smemb);
    memcpy(data, heap->data, heap->smemb);

    Kvp *kvp = kvp_construct(priority, data);

    if (heap->destructor != NULL)
        heap->destructor(heap->data);

    memcpy(heap->data, heap->data + heap->smemb * --heap->len, heap->smemb);
    heap->priorities[0] = heap->priorities[heap->len];

    __heap_heapify_down(heap);

    return kvp;
}

void heap_destroy(Heap *heap) {
    if (heap->destructor != NULL)
        for (size_t i = 0; i < heap->len; i++)
            heap->destructor(heap->data + heap->smemb * i);

    free(heap->data);
    free(heap->priorities);
    free(heap);
}
