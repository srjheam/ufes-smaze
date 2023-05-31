
#include <stdio.h>
#include <stdlib.h>
#include "src/search/labirinto.h"
#include "src/search/algorithms.h"

void print_result(ResultData *result)
{
    if (!result->sucesso)
    {
        printf("IMPOSSIVEL\n");
        return;
    }

    for (int i = 0; i < result->tamanho_caminho; i++)
        printf("%d %d\n", result->caminho[i].x, result->caminho[i].y);

    printf("%.2lf\n", result->custo_caminho);
    printf("%d\n", result->tamanho_caminho);
    printf("%d\n", result->nos_expandidos);
}

void mostra_caminho(Labirinto *l, ResultData *result, Celula inicio, Celula fim)
{
    if (result->sucesso)
    {
        for (int i = 0; i < result->tamanho_caminho; i++)
            labirinto_atribuir(l, result->caminho[i].y, result->caminho[i].x, CAMINHO);
    }

    labirinto_atribuir(l, inicio.y, inicio.x, INICIO);
    labirinto_atribuir(l, fim.y, fim.x, FIM);
    labirinto_print(l);
}

int main()
{
    char arquivo_labirinto[100];
    char algoritmo[100];
    Celula inicio, fim;
    ResultData result;
    Labirinto *lab;

    scanf("%s", arquivo_labirinto);
    scanf("%d %d", &inicio.x, &inicio.y);
    scanf("%d %d", &fim.x, &fim.y);
    scanf("\n%s", algoritmo);

    lab = labirinto_carregar(arquivo_labirinto);

    result = dummy_search(lab, inicio, fim);
    print_result(&result);
    mostra_caminho(lab, &result, inicio, fim);

    labirinto_destruir(lab);
    if (result.caminho != NULL)
        free(result.caminho);

    return 0;
}