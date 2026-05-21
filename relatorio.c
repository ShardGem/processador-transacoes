#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transacao.h"
#include "fila.h"
#include "pilha.h"
#include "lista_risco.h"

void gerar_relatorio(
    const char *bin_path,
    double tempo_ordenacao,
    Fila *fila_autorizacao, // <-- haha um pointer mo coisa de nerd
    Pilha *pilha_rejeicao,
    ListaRisco *lista_suspeitas)
{
    // Reabrir o arquivo em binário para pegar as métricas financeiras
    // COMO QUE MEXE COM ESSA PORRA DE ARQUIVO BINARIO PQP
    FILE *bin = fopen(bin_path, "rb");
    if (!bin) {
        perror("Erro ao abrir binário para relatório AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
        return;
    }

    float total_visa = 0, total_master = 0;
    Transacao maior_t;
    maior_t.valor = -1;
    maior_t.id = 0;

    Transacao t;
    while (fread(&t, sizeof(Transacao), 1, bin)) {
        // total por bandeira
        // Comparação sem sensibilidade de maiúscula pra não dar problema
        if (strcasecmp(t.bandeira, "Visa") == 0) {
            total_visa += t.valor;
        } else if (strcasecmp(t.bandeira, "MasterCard") == 0 || 
                strcasecmp(t.bandeira, "Mastercard") == 0) {
            total_master += t.valor;
        }
        // maior transação
        if (t.valor > maior_t.valor) {
            maior_t = t;
        }
    }
    fclose(bin);

    FILE *rel = fopen("relatorio_consolidado.txt", "w");
    if (!rel) {
        perror("Erro ao criar relatório");
        return;
    }

    fprintf(rel, "==================================================\n");
    fprintf(rel, "RELATÓRIO DE PERFORMANCE E AUDITORIA DE TRANSAÇÕES\n");
    fprintf(rel, "==================================================\n\n");

    fprintf(rel, "1. DESEMPENHO DA ORDENAÇÃO\n");
    fprintf(rel, "  - Tempo gasto para ordenar 100k registros: %.2f segundos.\n\n", tempo_ordenacao);

    fprintf(rel, "2. MÉTRICAS FINANCEIRAS\n");
    fprintf(rel, "  - Total processado por Bandeira (Visa): R$ %.2f\n", total_visa);
    fprintf(rel, "  - Total processado por Bandeira (Mastercard): R$ %.2f\n", total_master);
    fprintf(rel, "  - Maior transação detectada: ID %lu - Valor: R$ %.2f\n\n", maior_t.id, maior_t.valor);

    fprintf(rel, "3. STATUS DAS ESTRUTURAS\n");
    fprintf(rel, "  - Transações processadas na Fila (FIFO): %d transações.\n", fila_autorizacao->tamanho);
    fprintf(rel, "  - Transações armazenadas na Pilha de Rejeição: %d transações.\n", pilha_rejeicao->tamanho);
    fprintf(rel, "  - Transações críticas na Lista de Suspeitas: %d transações.\n\n", lista_suspeitas->tamanho);

    fprintf(rel, "==================================================\n");
    fprintf(rel, "LISTA DE TRANSAÇÕES SUSPEITAS (VALOR ALTO + MADRUGADA)\n");
    fprintf(rel, "==================================================\n");

    imprime_lista_risco(lista_suspeitas, rel);

    fprintf(rel, "\n--- Fim do relatório ---\n");
    fclose(rel);

    printf("Relatório gerado: relatorio_consolidado.txt\n");
}