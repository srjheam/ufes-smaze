#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deque.h"

struct Celula {
    int x;
    int y;
};

int main(int argc, char const *argv[]) {
    int n;
    scanf("%d", &n);

    Deque *d = deque_construct(sizeof(struct Celula), NULL);
    while (n--) {
        char op[11];
        struct Celula celula;
        scanf("%11s", op);

        if (strcmp(op, "POP_BACK") == 0) {
            struct Celula *celula = deque_pop_back(d);
            printf("%d %d\n", celula->x, celula->y);
            free(celula);
        } else if (strcmp(op, "POP_FRONT") == 0) {
            struct Celula *celula = deque_pop_front(d);
            printf("%d %d\n", celula->x, celula->y);
            free(celula);
        } else {
            scanf("%d %d", &celula.x, &celula.y);

            if (strcmp(op, "PUSH_BACK") == 0) {
                deque_push_back(d, &celula);
            } else if (strcmp(op, "PUSH_FRONT") == 0) {
                deque_push_front(d, &celula);
            }
        }
    }

    deque_destroy(d);

    return EXIT_SUCCESS;
}
