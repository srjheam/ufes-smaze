
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algorithms.h"
#include "celula.h"
#include "deque.h"
#include "labirinto.h"

void print_result(ResultData *result) {
    if (!result->sucesso) {
        printf("IMPOSSIVEL\n");
        return;
    }

    int n = deque_size(result->caminho);
    for (int i = n - 1; i >= 0; i--)
        printf("%ld %ld\n",
               celula_get_y(*(Celula **)deque_get(result->caminho, i)),
               celula_get_x(*(Celula **)deque_get(result->caminho, i)));

    printf("%.2lf\n", result->custo_caminho);
    printf("%d\n", result->tamanho_caminho);
    printf("%d\n", result->nos_expandidos);

    deque_destroy(result->caminho);
}

int main() {
    char arquivo_labirinto[100];
    char algoritmo[100];
    int x0, y0, xf, yf;
    ResultData result;
    Labirinto *lab;

    scanf("%s", arquivo_labirinto);
    scanf("%d %d", &y0, &x0);
    scanf("%d %d", &yf, &xf);
    scanf("\n%s", algoritmo);

    Celula *inicio = celula_construct(x0, y0);
    Celula *fim = celula_construct(xf, yf);

    lab = labirinto_carregar(arquivo_labirinto);

    if (!strcmp(algoritmo, "BFS"))
        result = breadth_first_search(lab, inicio, fim);
    else if (!strcmp(algoritmo, "DFS"))
        result = depth_first_search(lab, inicio, fim);
    else if (!strcmp(algoritmo, "A*"))
        result = a_star(lab, inicio, fim);
    else {
        printf("Algoritmo desconhecido: %s\n", algoritmo);
        exit(1);
    }

    celula_destroy(inicio);
    celula_destroy(fim);

    print_result(&result);

    // descomente para visualizar informacoes de debug no labirinto
    // mostra_caminho(lab, &result, inicio, fim);

    labirinto_destruir(lab);

    return 0;
}
