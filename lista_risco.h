#ifndef LISTA_RISCO_H
#define LISTA_RISCO_H

#include "transacao.h"

typedef struct NoRisco {
    Transacao dado;
    struct NoRisco *prox;
} NoRisco;

typedef struct {
    NoRisco *cabeca;
    int tamanho;
} ListaRisco;

ListaRisco* cria_lista_risco();
void insere_risco(ListaRisco *l, Transacao t);
void imprime_lista_risco(ListaRisco *l, FILE *out);  // parao relatório
void libera_lista_risco(ListaRisco *l);

#endif