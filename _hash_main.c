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

int cell_cmp(struct Celula *a, struct Celula *b) {
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    return dx * dx + dy * dy;
}

int main(int argc, char const *argv[]) {
    int n;
    scanf("%d", &n);

    Hashtable *h = ht_construct(n, 0.75, (hash_fn)cell_hash, (cmp_fn)cell_cmp, free, free);
    while (n--) {
        char op[5];
        scanf("%5s", op);

        if (strcmp(op, "GET") == 0) {
            Kvp *kvp = heap_pop(h);
            double *priority = kvp_key(kvp);
            struct Celula *celula = kvp_value(kvp);

            printf("%d %d %.0f\n", celula->x, celula->y, *priority);

            free(celula);
            free(priority);
            kvp_destroy(kvp);
        } else {
            struct Celula celula;
            int prioridade;
            scanf("%d %d %d", &celula.x, &celula.y, &prioridade);

            heap_push(h, &celula, prioridade);
        }
    }

    heap_destroy(h);

    return EXIT_SUCCESS;
}
