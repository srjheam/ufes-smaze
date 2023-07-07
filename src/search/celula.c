#include "celula.h"

struct Celula {
    size_t x;
    size_t y;
};

Celula *celula_construct(size_t x, size_t y) {
    Celula *self = malloc(sizeof(Celula));
    self->x = x;
    self->y = y;
    return self;
}

size_t celula_get_x(Celula *self) { return self->x; }

size_t celula_get_y(Celula *self) { return self->y; }

void celula_set_x(Celula *self, size_t x) { self->x = x; }

void celula_set_y(Celula *self, size_t y) { self->y = y; }

size_t celula_hash(Celula *key) {
    return (key->x * 73856093) ^ (key->y * 83492791);
}

int celula_cmp(Celula *a, Celula *b) {
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    return dx * dx + dy * dy;
}

Celula *celula_copy(Celula *self) {
    Celula *nc = celula_construct(celula_get_x(self), celula_get_y(self));

    return nc;
}

void celula_destroy(Celula *self) { free(self); }
