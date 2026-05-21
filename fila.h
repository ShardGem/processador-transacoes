#ifndef FILA_H
#define FILA_H

#include "transacao.h"

typedef struct NoFila {
    Transacao dado;
    struct NoFila *prox;
} NoFila;

typedef struct {
    NoFila *inicio;
    NoFila *fim;
    int tamanho;
} Fila;

Fila* cria_fila();
void enfileira(Fila *f, Transacao t);
Transacao desenfileira(Fila *f);
int fila_vazia(Fila *f);
void libera_fila(Fila *f);

#endif