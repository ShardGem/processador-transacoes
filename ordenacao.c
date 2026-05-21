#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ordenacao.h"

// -------- Funções de comparação --------

// Comparação para valor decrescente
int cmp_valor(const void *a, const void *b) {
    Transacao *ta = (Transacao *)a;
    Transacao *tb = (Transacao *)b;
    if (tb->valor > ta->valor) return 1;
    if (tb->valor < ta->valor) return -1;
    return 0;
}

// Comparação para data+hora crescente
// data no formato DD/MM/AAAA -> podemos converter para string AAAAMMDDHHMMSS
void datahora_para_str(char *buffer, const char *data, const char *hora) {
    // data: DD/MM/AAAA -> buffer: AAAAMMDD
    // hora: HH:MM:SS -> adiciona HHMMSS
    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            data[6], data[7], data[8], data[9], // ano
            data[3], data[4],                   // mês
            data[0], data[1],                   // dia
            hora[0], hora[1],                   // hora
            hora[3], hora[4],                   // min
            hora[6], hora[7]);                  // seg
    buffer[14] = '\0';
}

int cmp_cronologia(const void *a, const void *b) {
    Transacao *ta = (Transacao *)a;
    Transacao *tb = (Transacao *)b;
    char stra[15], strb[15];
    datahora_para_str(stra, ta->data, ta->hora);
    datahora_para_str(strb, tb->data, tb->hora);
    return strcmp(stra, strb);
}

// ---------- Algoritmos de ordenação (QuickSort do proprio C ebaaa) ----------

void ordenar_por_valor(Transacao *vetor, int n) {
    qsort(vetor, n, sizeof(Transacao), cmp_valor);
}

void ordenar_por_cronologia(Transacao *vetor, int n) {
    qsort(vetor, n, sizeof(Transacao), cmp_cronologia);
}