#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "transacao.h"
#include "conversor.h"
#include "ordenacao.h"
#include "fila.h"
#include "pilha.h"
#include "lista_risco.h"
#include "relatorio.h"

#include <locale.h> // Linha ajustar pra UTF-8

#define VALOR_CORTE_RISCO 300.0   // Coloquei o valor de corte pra ser transaçãoi suspeita como > 200, mas pode mudar se precisar
#define HORA_INICIO_RISCO "23:00:00"
#define HORA_FIM_RISCO "05:00:00"

// Função para verificar se horário está no intervalo de madrugada
int horario_madrugada(const char *hora) {
    // converte para inteiro (HH:MM:SS) ou compara strings
    return (strcmp(hora, "23:00:00") >= 0 || strcmp(hora, "05:00:00") <= 0);
}

int main() {
    setlocale(LC_ALL, "Portuguese"); // Adicione esta linha

    // 1 - Converter CSV pra Binário
    int num_transacoes = csv_para_binario("CreditCardData.csv", "transacoes.dat");
    if (num_transacoes <= 0) {
        printf("Nenhuma transação convertida. Encerrando.\n");
        return 1;
    }

    // 2 - Carregar em vetor para ordenação (siomular base indexada blablabla)
    FILE *bin = fopen("transacoes.dat", "rb");
    if (!bin) {
        perror("Erro ao abrir binário");
        return 1;
    }

    // Contar registros (já sabemos num_transacoes, mas pode obter do arquivo)
    fseek(bin, 0, SEEK_END);
    long tamanho_arquivo = ftell(bin);
    int total_registros = tamanho_arquivo / sizeof(Transacao);
    rewind(bin);

    Transacao *vetor = (Transacao *)malloc(total_registros * sizeof(Transacao));
    if (!vetor) {
        printf("Erro de alocação de memória.\n");
        fclose(bin);
        return 1;
    }

    fread(vetor, sizeof(Transacao), total_registros, bin);
    fclose(bin);

    // 3 - Faz as ordenações (cronológica primeiro, só demonstração)
    clock_t inicio = clock();
    ordenar_por_cronologia(vetor, total_registros);
    clock_t fim = clock();
    double tempo_cron = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // (Opcional: ordenar por valor, mas não necessário para o fluxo)
    // ordenar_por_valor(vetor, total_registros);

    // Etapa 4: Criar estruturas lineares
    Fila *fila_autorizacao = cria_fila();
    Pilha *pilha_rejeicao = cria_pilha();
    ListaRisco *lista_suspeitas = cria_lista_risco();

    // Processar transações (simulação do pipeline)
    for (int i = 0; i < total_registros; i++) {
        Transacao t = vetor[i];

        // Regra de risco: valor > corte E horário entre 23:00 e 05:00
        if (t.valor > VALOR_CORTE_RISCO && horario_madrugada(t.hora)) {
            insere_risco(lista_suspeitas, t);
        }

        // Lógica de autorização (simplificada)
        if (t.status == 1) {  // Aprovada (sem fraude no CSV)
            // Entra na fila de autorização (validação de saldo)
            enfileira(fila_autorizacao, t);
        } else {
            // Rejeitada (fraud=1) -> empilha na pilha de rejeição para auditoria
            empilha(pilha_rejeicao, t);
        }
    }

    // Etapa 5: Gerar relatório consolidado
    gerar_relatorio("transacoes.dat", tempo_cron,
                    fila_autorizacao, pilha_rejeicao, lista_suspeitas);

    // Etapa 6: Liberar memória (free obrigatório)
    free(vetor);
    libera_fila(fila_autorizacao);
    libera_pilha(pilha_rejeicao);
    libera_lista_risco(lista_suspeitas);

    printf("Processamento concluído com sucesso.\n");
    return 0;
}