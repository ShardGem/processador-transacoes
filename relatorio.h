#ifndef RELATORIO_H
#define RELATORIO_H

void gerar_relatorio(
    const char *bin_path,
    double tempo_ordenacao,
    Fila *fila_autorizacao,
    Pilha *pilha_rejeicao,
    ListaRisco *lista_suspeitas
);

#endif