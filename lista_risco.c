#include <stdio.h>
#include <stdlib.h>
#include "lista_risco.h"

ListaRisco* cria_lista_risco() {
    ListaRisco *l = (ListaRisco *)malloc(sizeof(ListaRisco));
    l->cabeca = NULL;
    l->tamanho = 0;
    return l;
}

void insere_risco(ListaRisco *l, Transacao t) {
    NoRisco *novo = (NoRisco *)malloc(sizeof(NoRisco));
    novo->dado = t;
    novo->prox = l->cabeca;
    l->cabeca = novo;
    l->tamanho++;
}

void imprime_lista_risco(ListaRisco *l, FILE *out) {
    NoRisco *atual = l->cabeca;
    while (atual != NULL) {
        fprintf(out, "[ID: %lu] - Data: %s %s - Valor: R$ %.2f - Categoria: %s\n",
                atual->dado.id, atual->dado.data, atual->dado.hora,
                atual->dado.valor, atual->dado.categoria);
        atual = atual->prox;
    }
}

void libera_lista_risco(ListaRisco *l) {
    NoRisco *atual = l->cabeca;
    while (atual != NULL) {
        NoRisco *tmp = atual;
        atual = atual->prox;
        free(tmp);
    }
    free(l);
}