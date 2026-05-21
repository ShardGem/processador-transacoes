#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

Fila* cria_fila() {
    Fila *f = (Fila *)malloc(sizeof(Fila));
    f->inicio = f->fim = NULL;
    f->tamanho = 0;
    return f;
}

void enfileira(Fila *f, Transacao t) {
    NoFila *novo = (NoFila *)malloc(sizeof(NoFila));
    novo->dado = t;
    novo->prox = NULL;
    if (f->fim == NULL) {
        f->inicio = f->fim = novo;
    } else {
        f->fim->prox = novo;
        f->fim = novo;
    }
    f->tamanho++;
}

Transacao desenfileira(Fila *f) {
    Transacao vazia = {0};
    if (fila_vazia(f)) return vazia;
    NoFila *rem = f->inicio;
    Transacao ret = rem->dado;
    f->inicio = rem->prox;
    if (f->inicio == NULL) f->fim = NULL;
    free(rem);
    f->tamanho--;
    return ret;
}

int fila_vazia(Fila *f) {
    return f->inicio == NULL;
}

void libera_fila(Fila *f) {
    while (!fila_vazia(f)) {
        desenfileira(f);
    }
    free(f);
}