
#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include "celula.h"
#include "labirinto.h"

#include "deque.h"

typedef struct
{
    Deque *caminho;
    int tamanho_caminho;
    int nos_expandidos;
    double custo_caminho;
    int sucesso;
} ResultData;

ResultData a_star(Labirinto *l, Celula *inicio, Celula *fim);
ResultData breadth_first_search(Labirinto *l, Celula *inicio, Celula *fim);
ResultData depth_first_search(Labirinto *l, Celula *inicio, Celula *fim);

#endif