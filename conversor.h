#ifndef CONVERSOR_H
#define CONVERSOR_H

#include "transacao.h"   // definição da struct Transacao

// Eu lembro da primeira vez que eu aprendi sobre structs com o Wilson, foi poggers

// Lê o arquivo CSV e grava registros válidos no arquivo binário.
// Retorna o número de transações convertidas.
int csv_para_binario(const char *csv_path, const char *bin_path);

#endif