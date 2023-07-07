#ifndef _CELULA_H_
#define _CELULA_H_

#include <stdlib.h>

typedef struct Celula Celula;

Celula *celula_construct(size_t x, size_t y);

size_t celula_get_x(Celula *self);

size_t celula_get_y(Celula *self);

void celula_set_x(Celula *self, size_t x);

void celula_set_y(Celula *self, size_t y);

size_t celula_hash(Celula *key);

int celula_cmp(Celula *a, Celula *b);

Celula *celula_copy(Celula *self);

void celula_destroy(Celula *self);

#endif
