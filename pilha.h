#ifndef PILHA_H
#define PILHA_H

#include "transacao.h"

typedef struct NoPilha {
    Transacao dado;
    struct NoPilha *prox;
} NoPilha;

typedef struct {
    NoPilha *topo;
    int tamanho;
} Pilha;

Pilha* cria_pilha();
void empilha(Pilha *p, Transacao t);
Transacao desempilha(Pilha *p);  // função undo()
int pilha_vazia(Pilha *p);
void libera_pilha(Pilha *p);

#endif