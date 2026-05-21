#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "transacao.h"

// Ordena vetor de transações por valor decrescente (1o ceiterio)
void ordenar_por_valor(Transacao *vetor, int n);

// Ordena vetor de transações por data+hora crescente (2o criterio)
void ordenar_por_cronologia(Transacao *vetor, int n);

#endif