#ifndef _ASTAR_MAP_
#define _ASTAR_MAP_

#include <stdlib.h>
#include <stdbool.h>

#include "celula.h"
#include "key_value_pair.h"

typedef struct AStarMap AStarMap;

AStarMap *astarmap_construct(size_t n, size_t m);

double astarmap_get(AStarMap *self, size_t x, size_t y);

/**
 * @brief Set the distance of a node in the map.
 *
 * @param self The map.
 * @param x Map node x coordinate.
 * @param y Map node y coordinate.
 * @param distance The distance to set.
 * @return true If the node was set.
 */
Celula *astarmap_set(AStarMap *self, Celula *cell, double distance);

Kvp *astarmap_peek_shortest(AStarMap *self);

Kvp *astarmap_pop_shortest(AStarMap *self);

size_t astarmap_len(AStarMap *self);

void astarmap_destroy(AStarMap *self);

#endif
