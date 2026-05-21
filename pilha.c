#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

Pilha* cria_pilha() {
    Pilha *p = (Pilha *)malloc(sizeof(Pilha));
    p->topo = NULL;
    p->tamanho = 0;
    return p;
}

void empilha(Pilha *p, Transacao t) {
    NoPilha *novo = (NoPilha *)malloc(sizeof(NoPilha));
    novo->dado = t;
    novo->prox = p->topo;
    p->topo = novo;
    p->tamanho++;
}

Transacao desempilha(Pilha *p) {
    Transacao vazia = {0};
    if (pilha_vazia(p)) return vazia;
    NoPilha *rem = p->topo;
    Transacao ret = rem->dado;
    p->topo = rem->prox;
    free(rem);
    p->tamanho--;
    return ret;
}

int pilha_vazia(Pilha *p) {
    return p->topo == NULL;
}

void libera_pilha(Pilha *p) {
    while (!pilha_vazia(p)) {
        desempilha(p);
    }
    free(p);
}