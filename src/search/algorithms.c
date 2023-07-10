
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

int _is_astar_free(Labirinto *l, size_t x, size_t y) {
    return _inside_bounds(l, x, y) && (labirinto_obter(l, y, x) == LIVRE ||
                                       labirinto_obter(l, y, x) == FRONTEIRA);
}

double _distance(Celula *a, Celula *b) {
    return sqrt(pow(celula_get_x(a) - celula_get_x(b), 2) +
                pow(celula_get_y(a) - celula_get_y(b), 2));
}

ResultData _default_result() {
    ResultData result;

    result.tail = NULL;
    result.nos_expandidos = 0;
    result.sucesso = 0;

    return result;
}

ResultData a_star(Labirinto *l, Celula *inicio, Celula *fim) {
    ResultData result = _default_result();

    AStarMap *map =
        astarmap_construct(labirinto_n_linhas(l), labirinto_n_colunas(l));

    astarmap_set(map, inicio, 0);

    while (astarmap_len(map) > 0) {
        labirinto_print(l);
        printf("\n");

        result.nos_expandidos++;
        Kvp *kvp = astarmap_pop_shortest(map);

        Celula *current = kvp_value(kvp);

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
            double f1 = _distance(current, c);
            double f2 = _distance(c, fim);
            double f = _distance(current, c) + _distance(c, fim);
            double r = astarmap_set(map, c, f);
            if (f == r) {
                celula_set_accCost(c, celula_get_accCost(current) +
                                          _distance(current, c));
                celula_set_parent(c, current);
            }
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 2
        if (_is_free(l, celula_get_x(current) + 1, celula_get_y(current) - 1)) {
            Celula *c = celula_construct(celula_get_x(current) + 1,
                                         celula_get_y(current) - 1);
            double f1 = _distance(current, c);
            double f2 = _distance(c, fim);
            double f = _distance(current, c) + _distance(c, fim);
            double r = astarmap_set(map, c, f);
            if (f == r) {
                celula_set_accCost(c, celula_get_accCost(current) +
                                          _distance(current, c));
                celula_set_parent(c, current);
            }
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 3
        if (_is_free(l, celula_get_x(current) + 1, celula_get_y(current))) {
            Celula *c = celula_construct(celula_get_x(current) + 1,
                                         celula_get_y(current));
            double f1 = _distance(current, c);
            double f2 = _distance(c, fim);
            double f = _distance(current, c) + _distance(c, fim);
            double r = astarmap_set(map, c, f);
            if (f == r) {
                celula_set_accCost(c, celula_get_accCost(current) +
                                          _distance(current, c));
                celula_set_parent(c, current);
            }
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 4
        if (_is_free(l, celula_get_x(current) + 1, celula_get_y(current) + 1)) {
            Celula *c = celula_construct(celula_get_x(current) + 1,
                                         celula_get_y(current) + 1);
            double f1 = _distance(current, c);
            double f2 = _distance(c, fim);
            double f = _distance(current, c) + _distance(c, fim);
            double r = astarmap_set(map, c, f);
            if (f == r) {
                celula_set_accCost(c, celula_get_accCost(current) +
                                          _distance(current, c));
                celula_set_parent(c, current);
            }
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 5
        if (_is_free(l, celula_get_x(current), celula_get_y(current) + 1)) {
            Celula *c = celula_construct(celula_get_x(current),
                                         celula_get_y(current) + 1);
            double f1 = _distance(current, c);
            double f2 = _distance(c, fim);
            double f = _distance(current, c) + _distance(c, fim);
            double r = astarmap_set(map, c, f);
            if (f == r) {
                celula_set_accCost(c, celula_get_accCost(current) +
                                          _distance(current, c));
                celula_set_parent(c, current);
            }
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 6
        if (_is_free(l, celula_get_x(current) - 1, celula_get_y(current) + 1)) {
            Celula *c = celula_construct(celula_get_x(current) - 1,
                                         celula_get_y(current) + 1);
            double f1 = _distance(current, c);
            double f2 = _distance(c, fim);
            double f = _distance(current, c) + _distance(c, fim);
            double r = astarmap_set(map, c, f);
            if (f == r) {
                celula_set_accCost(c, celula_get_accCost(current) +
                                          _distance(current, c));
                celula_set_parent(c, current);
            }
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 7
        if (_is_free(l, celula_get_x(current) - 1, celula_get_y(current))) {
            Celula *c = celula_construct(celula_get_x(current) - 1,
                                         celula_get_y(current));
            double f1 = _distance(current, c);
            double f2 = _distance(c, fim);
            double f = _distance(current, c) + _distance(c, fim);
            double r = astarmap_set(map, c, f);
            if (f == r) {
                celula_set_accCost(c, celula_get_accCost(current) +
                                          _distance(current, c));
                celula_set_parent(c, current);
            }
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 8
        if (_is_free(l, celula_get_x(current) - 1, celula_get_y(current) - 1)) {
            Celula *c = celula_construct(celula_get_x(current) - 1,
                                         celula_get_y(current) - 1);
            double f1 = _distance(current, c);
            double f2 = _distance(c, fim);
            double f = _distance(current, c) + _distance(c, fim);
            double r = astarmap_set(map, c, f);
            if (f == r) {
                celula_set_accCost(c, celula_get_accCost(current) +
                                          _distance(current, c));
                celula_set_parent(c, current);
            }
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }
    }

    return result;
}

ResultData breadth_first_search(Labirinto *l, Celula *inicio, Celula *fim) {
    ResultData result = _default_result();

    Queue *frontier = queue_construct(__SIZEOF_POINTER__, NULL);

    queue_push(frontier, &inicio);

    while (!queue_empty(frontier)) {
        result.nos_expandidos++;
        Celula **popc = queue_pop(frontier);
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
            queue_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 2
        if (_is_free(l, celula_get_x(current) + 1, celula_get_y(current) - 1)) {
            Celula *c = celula_construct(celula_get_x(current) + 1,
                                         celula_get_y(current) - 1);
            celula_set_parent(c, current);
            queue_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 3
        if (_is_free(l, celula_get_x(current) + 1, celula_get_y(current))) {
            Celula *c = celula_construct(celula_get_x(current) + 1,
                                         celula_get_y(current));
            celula_set_parent(c, current);
            queue_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 4
        if (_is_free(l, celula_get_x(current) + 1, celula_get_y(current) + 1)) {
            Celula *c = celula_construct(celula_get_x(current) + 1,
                                         celula_get_y(current) + 1);
            celula_set_parent(c, current);
            queue_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 5
        if (_is_free(l, celula_get_x(current), celula_get_y(current) + 1)) {
            Celula *c = celula_construct(celula_get_x(current),
                                         celula_get_y(current) + 1);
            celula_set_parent(c, current);
            queue_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 6
        if (_is_free(l, celula_get_x(current) - 1, celula_get_y(current) + 1)) {
            Celula *c = celula_construct(celula_get_x(current) - 1,
                                         celula_get_y(current) + 1);
            celula_set_parent(c, current);
            queue_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 7
        if (_is_free(l, celula_get_x(current) - 1, celula_get_y(current))) {
            Celula *c = celula_construct(celula_get_x(current) - 1,
                                         celula_get_y(current));
            celula_set_parent(c, current);
            queue_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // 8
        if (_is_free(l, celula_get_x(current) - 1, celula_get_y(current) - 1)) {
            Celula *c = celula_construct(celula_get_x(current) - 1,
                                         celula_get_y(current) - 1);
            celula_set_parent(c, current);
            queue_push(frontier, &c);
            labirinto_atribuir(l, celula_get_y(c), celula_get_x(c), FRONTEIRA);
        }

        // labirinto_print(l);
        // printf("\n");
    }

    return result;
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

    return result;
}
