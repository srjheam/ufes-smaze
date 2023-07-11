#include "celula.h"

struct Celula {
    size_t x;
    size_t y;
    struct Celula *parent;
    double accCost;
};

Celula *celula_construct(size_t x, size_t y) {
    Celula *self = malloc(sizeof(Celula));
    self->x = x;
    self->y = y;
    self->parent = NULL;
    self->accCost = 0;
    return self;
}

size_t celula_get_x(Celula *self) { return self->x; }

size_t celula_get_y(Celula *self) { return self->y; }

void celula_set_x(Celula *self, size_t x) { self->x = x; }

void celula_set_y(Celula *self, size_t y) { self->y = y; }

Celula *celula_get_parent(Celula *self) { return self->parent; }

void celula_set_parent(Celula *self, Celula *parent) { self->parent = parent; }

double celula_get_accCost(Celula *self) { return self->accCost; }

void celula_set_accCost(Celula *self, double accCost) {
    self->accCost = accCost;
}

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

    celula_set_parent(nc, celula_get_parent(self));
    celula_set_accCost(nc, celula_get_accCost(self));

    return nc;
}

Celula *celula_dcopy(Celula *self) {
    if (self == NULL)
        return NULL;

    Celula *nc = celula_construct(celula_get_x(self), celula_get_y(self));

    celula_set_parent(nc, celula_dcopy(celula_get_parent(self)));
    celula_set_accCost(nc, celula_get_accCost(self));

    return nc;
}

void celula_destroy(Celula *self) { free(self); }

void celula_ddestroy(Celula *self) {
    if (self == NULL)
        return;

    celula_ddestroy(celula_get_parent(self));
    free(self);
}
