#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astarmap.h"

int main(int argc, char const *argv[]) {
    size_t n;
    scanf("%ld", &n);

    AStarMap *h = astarmap_construct(n, 1);
    while (n--) {
        char op[5];
        scanf("%5s", op);

        if (strcmp(op, "POP") == 0) {
            Kvp *kvp = astarmap_pop_shortest(h);
            if (kvp == NULL)
                continue;

            double *priority = kvp_key(kvp);
            Celula *celula = kvp_value(kvp);

            printf("%ld %ld %.0f\n", celula_get_x(celula), celula_get_y(celula),
                   *priority);

            free(celula);
            free(priority);
            kvp_destroy(kvp);
        } else {
            size_t x;
            size_t y;
            int prioridade;
            scanf("%ld %ld %d", &x, &y, &prioridade);

            Celula *c = celula_construct(x, y);
            astarmap_set(h, celula_get_x(c), celula_get_y(c), prioridade);
            celula_destroy(c);
        }
    }

    astarmap_destroy(h);

    return EXIT_SUCCESS;
}
