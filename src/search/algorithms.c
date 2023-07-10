
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "astarmap.h"
#include "labirinto.h"
#include "queue.h"
#include "stack.h"

#include "algorithms.h"

#define SQRT_2                                                                 \
    1.41421356237309504880168872420969807856967187537694807317667973799

int _was_visited(Labirinto *l, size_t x, size_t y) {
    return labirinto_obter(l, y, x) != LIVRE;
}

int _inside_bounds(Labirinto *l, size_t x, size_t y) {
    return x < (size_t)labirinto_n_colunas(l) &&
           y < (size_t)labirinto_n_linhas(l);
}

int _is_free(Labirinto *l, size_t x, size_t y) {
    return _inside_bounds(l, x, y) && !_was_visited(l, x, y);
}

double _distance(Celula *a, Celula *b) {
    return celula_get_x(a) == celula_get_x(b) ||
                   celula_get_y(a) == celula_get_y(b)
               ? 1
               : SQRT_2;
}

ResultData _default_result() {
    ResultData result;

    result.tail = NULL;
    result.nos_expandidos = 0;
    result.sucesso = 0;

    return result;
}

ResultData a_star(Labirinto *l, Celula *inicio, Celula *fim) {
    // TODO!
    return _default_result();
}

ResultData breadth_first_search(Labirinto *l, Celula *inicio, Celula *fim) {
    // TODO!
    return _default_result();
}

ResultData depth_first_search(Labirinto *l, Celula *inicio, Celula *fim) {
    ResultData result = _default_result();

    Stack *frontier = stack_construct(__SIZEOF_POINTER__, NULL);

    stack_push(frontier, &inicio);

    while (!stack_empty(frontier)) {
        result.nos_expandidos++;
        Celula **popc = stack_pop(frontier);
        Celula *current = *popc;
        free(popc);
        if (celula_get_parent(current) != NULL) {
            celula_set_accCost(
                current, celula_get_accCost(celula_get_parent(current)) +
                             _distance(current, celula_get_parent(current)));
        }

        if (celula_get_x(current) == celula_get_x(fim) &&
            celula_get_y(current) == celula_get_y(fim)) {
            result.tail = current;
            result.sucesso = 1;
            break;
        }

        labirinto_atribuir(l, celula_get_y(current), celula_get_x(current),
                           CAMINHO);

        //
        // 8 1 2
        // 7 O 3
        // 6 5 4
        //

        // 1
        if (_is_free(l, celula_get_x(current), celula_get_y(current) - 1)) {
            Celula *c = celula_construct(celula_get_x(current),
                                         celula_get_y(current) - 1);
            celula_set_parent(c, current);
            stack_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 2
        if (_is_free(l, celula_get_x(current) + 1, celula_get_y(current) - 1)) {
            Celula *c = celula_construct(celula_get_x(current) + 1,
                                         celula_get_y(current) - 1);
            celula_set_parent(c, current);
            stack_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 3
        if (_is_free(l, celula_get_x(current) + 1, celula_get_y(current))) {
            Celula *c = celula_construct(celula_get_x(current) + 1,
                                         celula_get_y(current));
            celula_set_parent(c, current);
            stack_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 4
        if (_is_free(l, celula_get_x(current) + 1, celula_get_y(current) + 1)) {
            Celula *c = celula_construct(celula_get_x(current) + 1,
                                         celula_get_y(current) + 1);
            celula_set_parent(c, current);
            stack_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 5
        if (_is_free(l, celula_get_x(current), celula_get_y(current) + 1)) {
            Celula *c = celula_construct(celula_get_x(current),
                                         celula_get_y(current) + 1);
            celula_set_parent(c, current);
            stack_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 6
        if (_is_free(l, celula_get_x(current) - 1, celula_get_y(current) + 1)) {
            Celula *c = celula_construct(celula_get_x(current) - 1,
                                         celula_get_y(current) + 1);
            celula_set_parent(c, current);
            stack_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 7
        if (_is_free(l, celula_get_x(current) - 1, celula_get_y(current))) {
            Celula *c = celula_construct(celula_get_x(current) - 1,
                                         celula_get_y(current));
            celula_set_parent(c, current);
            stack_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 8
        if (_is_free(l, celula_get_x(current) - 1, celula_get_y(current) - 1)) {
            Celula *c = celula_construct(celula_get_x(current) - 1,
                                         celula_get_y(current) - 1);
            celula_set_parent(c, current);
            stack_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // labirinto_print(l);
        // printf("\n");
    }

    stack_destroy(frontier);

    return result;
}
