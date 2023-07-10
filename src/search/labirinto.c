
#include <stdio.h>
#include <stdlib.h>

#include "labirinto.h"

struct Labirinto {
    int n_linhas;
    int n_colunas;
    unsigned char **celulas;
};

Labirinto *labirinto_carregar(char *arquivo) {
    FILE *file = fopen(arquivo, "rb");

    if (file == NULL)
        exit(printf("Arquivo %s nao encontrado.\n", arquivo));

    int n_linhas, n_colunas;
    fread(&n_linhas, sizeof(int), 1, file);
    fread(&n_colunas, sizeof(int), 1, file);

    Labirinto *lab = (Labirinto *)malloc(sizeof(Labirinto));
    lab->celulas = (unsigned char **)malloc(n_linhas * sizeof(unsigned char *));
    lab->n_linhas = n_linhas;
    lab->n_colunas = n_colunas;

    for (int i = 0; i < n_linhas; i++) {
        lab->celulas[i] =
            (unsigned char *)malloc(n_colunas * sizeof(unsigned char));
        fread(lab->celulas[i], sizeof(unsigned char), n_colunas, file);
    }

    fclose(file);
    return lab;
}

int labirinto_n_linhas(Labirinto *l) { return l->n_linhas; }

int labirinto_n_colunas(Labirinto *l) { return l->n_colunas; }

void labirinto_atribuir(Labirinto *l, int linha, int coluna, TipoCelula valor) {
    if (linha < 0 || linha >= l->n_linhas || coluna < 0 ||
        coluna >= l->n_colunas)
        exit(printf(
            "Posição (%d, %d) inválida no labirinto com tamanho (%d, %d).\n",
            linha, coluna, l->n_linhas, l->n_colunas));

    l->celulas[linha][coluna] = valor;
}

TipoCelula labirinto_obter(Labirinto *l, int linha, int coluna) {
    if (linha < 0 || linha >= l->n_linhas || coluna < 0 ||
        coluna >= l->n_colunas)
        exit(printf(
            "Posição (%d, %d) inválida no labirinto com tamanho (%d, %d).\n",
            linha, coluna, l->n_linhas, l->n_colunas));

    return l->celulas[linha][coluna];
}

void labirinto_destruir(Labirinto *l) {
    for (int i = 0; i < l->n_linhas; i++)
        free(l->celulas[i]);

    free(l->celulas);
    free(l);
}

void _labirinto_print_celula(unsigned char val) {

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

    switch (val) {
    case LIVRE:
        printf(". " ANSI_COLOR_RESET);
        break;
    case OCUPADO:
        printf("# " ANSI_COLOR_RESET);
        break;
    case FRONTEIRA:
        printf(ANSI_COLOR_YELLOW "F " ANSI_COLOR_RESET);
        break;
    case EXPANDIDO:
        printf("X " ANSI_COLOR_RESET);
        break;
    case CAMINHO:
        printf(ANSI_COLOR_CYAN "O " ANSI_COLOR_RESET);
        break;
    case INICIO:
        printf(ANSI_COLOR_GREEN "I " ANSI_COLOR_RESET);
        break;
    case FIM:
        printf(ANSI_COLOR_RED "[]" ANSI_COLOR_RESET);
        break;
    default:
        printf("Tipo invalido de celula.\n");
    }
}

void labirinto_print(Labirinto *l) {
    for (int i = 0; i < l->n_linhas; i++) {
        printf("| ");
        for (int j = 0; j < l->n_colunas; j++)
            _labirinto_print_celula(labirinto_obter(l, i, j));
        printf("|\n");
    }
}