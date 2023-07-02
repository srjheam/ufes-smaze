#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

struct Celula {
    int x;
    int y;
};

int main(int argc, char const *argv[]) {
    int n;
    scanf("%d", &n);

    Heap *h = heap_construct(true, sizeof(struct Celula), NULL);
    while (n--) {
        char op[5];
        scanf("%5s", op);

        if (strcmp(op, "POP") == 0) {
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
