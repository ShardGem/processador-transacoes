#ifndef TRANSACAO_H
#define TRANSACAO_H

typedef struct {
    unsigned long id;
    char data[11];      // DD/MM/AAAA
    char hora[9];       // HH:MM:SS
    char bandeira[20];
    char categoria[30];
    float valor;
    int status;         // 0: Pendente, 1: Aprovada, 2: Rejeitada
} Transacao;

#endif