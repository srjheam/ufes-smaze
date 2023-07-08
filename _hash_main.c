#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

struct Celula {
    int x;
    int y;
};

size_t cell_hash(struct Celula *key) {
    return (key->x * 73856093) ^ (key->y * 83492791);
}

struct Celula *cell_copy(struct Celula *key) {
    struct Celula *ncell = malloc(sizeof(struct Celula));
    *ncell = *key;
    return ncell;
}

int cell_cmp(struct Celula *a, struct Celula *b) {
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    return dx * dx + dy * dy;
}

int main(int argc, char const *argv[]) {
    int n;
    scanf("%d", &n);

    Hashtable *h = ht_construct(19, 0.75, (hash_fn)cell_hash, (cmp_fn)cell_cmp, (copy_fn)cell_copy, free, free);
    while (n--) {
        char op[5];
        scanf("%5s", op);

        if (strcmp(op, "GET") == 0) {
            struct Celula celula;
            scanf("%d %d", &celula.x, &celula.y);

            int *p = ht_lookup(h, &celula);

            printf("%d\n", *p);
        } else {
            struct Celula celula;
            int prioridade;
            scanf("%d %d %d", &celula.x, &celula.y, &prioridade);

            int *p = malloc(sizeof(int));
            *p = prioridade;
            ht_put(h, &celula, p);
        }
    }

    ht_clear(h);
    ht_destroy(h);

    return EXIT_SUCCESS;
}
