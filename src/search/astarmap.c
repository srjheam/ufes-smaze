#include <string.h>

#include "astarmap.h"

#include "celula.h"
#include "exceptions.h"
#include "hashtable.h"
#include "utils.h"

#define _HEAP_SIZINI 4

struct AStarMap {
    Hashtable *map;

    Celula **data;
    double *priorities;
    size_t capacity;
    size_t len;
};

/*
 *
 * Heap trash implementation synced with the hash table
 *
 */

void __starmap_heap_grow(AStarMap *self) {
    if (self->len == self->capacity) {
        self->capacity <<= 1;
        self->data = realloc(self->data, __SIZEOF_POINTER__ * self->capacity);
        self->priorities =
            realloc(self->priorities, sizeof(double) * self->capacity);
    }
}

double __starmap_heap_cmp(double a, double b) { return b - a; }

void __starmap_heap_swap(AStarMap *map, size_t i, size_t j) {
    if (i >= map->len || j >= map->len)
        exception_throw_index("__starmap_heap_swap - Index out of bounds");

    Celula *tmp = map->data[i];
    map->data[i] = map->data[j];
    map->data[j] = tmp;

    double tmp2 = map->priorities[i];
    map->priorities[i] = map->priorities[j];
    map->priorities[j] = tmp2;

    int *vi = malloc(sizeof(int));
    *vi = i;
    int *vj = malloc(sizeof(int));
    *vj = j;
    ht_put(map->map, map->data[i], vi);
    ht_put(map->map, map->data[j], vj);
}

/**
 * @brief Returns the diff priority of the child with the highest priority; if
 * the result is negative, the left child has the highest priority; if the
 * result is positive, the right child has the highest priority; if the result
 * is zero, the children have less priority than the parent.
 *
 * @param heap The heap.
 * @param i The index of the parent.
 * @return double
 */
double __starmap_heap_heapify_high(AStarMap *map, size_t i) {
    size_t lindx = 2 * i + 1;
    size_t rindx = 2 * i + 2;

    // I love Linux file permissions.
    int bits = 0b01 * (lindx >= map->len) + 0b10 * (rindx >= map->len);
    switch (bits) {
    case 0b01:;
        double rdiff =
            __starmap_heap_cmp(map->priorities[rindx], map->priorities[i]);
        return rdiff > 0 ? rdiff : 0;

    case 0b10:;
        double ldiff =
            __starmap_heap_cmp(map->priorities[lindx], map->priorities[i]);
        return ldiff > 0 ? ldiff * -1 : 0;

    case 0b11:
        return 0;
    }

    double pp = map->priorities[i];
    double pl = map->priorities[lindx];
    double pr = map->priorities[rindx];

    double ldiff = __starmap_heap_cmp(pl, pp);
    double rdiff = __starmap_heap_cmp(pr, pp);

    int bits2 = 0b01 * (ldiff <= 0) + 0b10 * (rdiff <= 0);
    switch (bits2) {
    case 0b01:
        return rdiff;

    case 0b10:
        return ldiff * -1;

    case 0b11:
        return 0;
    }

    return ldiff < rdiff ? rdiff : ldiff * -1;
}

void __starmap_heap_heapify_up(AStarMap *map) {
    if (map->len == 1)
        return;

    size_t icurr = (map->len >> 1) - 1;
    while (__starmap_heap_heapify_high(map, icurr)) {
        size_t ichild =
            2 * icurr + (__starmap_heap_heapify_high(map, icurr) < 0 ? 1 : 2);

        __starmap_heap_swap(map, icurr, ichild);

        if (icurr == 0)
            break;
        icurr = ((icurr + 1) >> 1) - 1;
    }
}

void __starmap_heap_heapify_down(AStarMap *map) {
    size_t icurr = 0;
    while (__starmap_heap_heapify_high(map, icurr)) {
        size_t ichild =
            2 * icurr + (__starmap_heap_heapify_high(map, icurr) < 0 ? 1 : 2);

        __starmap_heap_swap(map, icurr, ichild);

        icurr = ichild;
    }
}

void __starmap_heap_push(AStarMap *map, Celula *data, double priority) {
    __starmap_heap_grow(map);

    map->priorities[map->len] = priority;
    map->data[map->len++] = data;

    int *vi = malloc(sizeof(int));
    *vi = map->len - 1;
    ht_put(map->map, data, vi);

    __starmap_heap_heapify_up(map);
}

bool __starmap_heap_empty(AStarMap *map) { return map->len == 0; }

Kvp *__starmap_heap_peek(AStarMap *map) {
    Kvp *kvp = kvp_construct(map->data[0], map->priorities);

    return kvp;
}

Kvp *__starmap_heap_pop(AStarMap *astar) {
    if (astar->len == 0)
        exception_throw_index("heap_pop - Heap is empty");

    double *priority = malloc(sizeof(double));
    *priority = astar->priorities[0];

    Kvp *kvp = kvp_construct(astar->data[0], priority);

    int *val = ht_delete(astar->map, astar->data[0]);
    free(val);

    __starmap_heap_swap(astar, 0, astar->len - 1);
    astar->len--;

    __starmap_heap_heapify_down(astar);

    return kvp;
}

AStarMap *astarmap_construct(size_t n, size_t m) {
    AStarMap *heap = malloc(sizeof *heap);

    heap->map = ht_construct(
        utils_neat_ht_initial_capacity_estimator(n * m), .75,
        (hash_fn)celula_hash, (cmp_fn)celula_cmp, (copy_fn)celula_copy,
        (destructor_fn)celula_destroy, (destructor_fn)free);

    heap->capacity = _HEAP_SIZINI;
    heap->data = malloc(__SIZEOF_POINTER__ * heap->capacity);
    heap->priorities = malloc(sizeof(double) * heap->capacity);
    heap->len = 0;

    return heap;
}

double *astarmap_get(AStarMap *self, size_t x, size_t y) {
    Celula *cel = celula_construct(x, y);
    double *d = ht_lookup(self->map, cel);
    free(cel);

    return d;
}

void astarmap_set(AStarMap *self, size_t x, size_t y, double distance) {
    Celula *cel = celula_construct(x, y);
    double *d = malloc(sizeof(double));
    *d = distance;
    ht_put(self->map, cel, d);
    free(cel);
}

Kvp *astarmap_peek_shortest(AStarMap *self) {
    return __starmap_heap_peek(self);
}

Kvp *astarmap_pop_shortest(AStarMap *self) { return __starmap_heap_pop(self); }

void astarmap_destroy(AStarMap *self) {
    ht_clear(self->map);
    ht_destroy(self->map);

    for (size_t i = 0; i < self->len; i++)
        celula_destroy(self->data[i]);

    free(self->data);
    free(self->priorities);
    free(self);
}
