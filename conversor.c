#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "conversor.h"

#define LINHA_MAX 512

// Mapeia mês abreviado (inglês) para número de gente normal (1-12)
int mes_para_numero(const char *mes) {
    const char *meses[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    for (int i = 0; i < 12; i++) {
        if (strcasecmp(mes, meses[i]) == 0) return i + 1;
    }
    return -1;
}

// Converte data "DD-Mon-YY" para "DD/MM/AAAA"
void converter_data(char *dest, const char *origem) {
    char dia_str[3], mes_str[4], ano_str[3];
    // Exemplo: "14-Oct-20"
    if (sscanf(origem, "%2[^-]-%3[^-]-%2s", dia_str, mes_str, ano_str) == 3) {
        int dia = atoi(dia_str);
        int mes = mes_para_numero(mes_str);
        int ano = atoi(ano_str) + 2000; // assume 20xx
        if (mes >= 1 && mes <= 12) {
            sprintf(dest, "%02d/%02d/%04d", dia, mes, ano);
            return;
        }
    }
    // fallback: copia original (não deveria acontecer)
    strncpy(dest, origem, 10);
    dest[10] = '\0';
}

// Converte hora "HH" para "HH:00:00"
void converter_hora(char *dest, const char *origem) {
    int hora = atoi(origem);
    if (hora >= 0 && hora <= 23) {
        sprintf(dest, "%02d:00:00", hora);
    } else {
        strncpy(dest, "00:00:00", 9);
    }
}

// Extrai dígitos de uma string (remove #, espaços, e etc)
unsigned long extrair_id(const char *str) {
    char numeros[20] = "";
    int j = 0;
    for (int i = 0; str[i]; i++) {
        if (isdigit((unsigned char)str[i])) {
            numeros[j++] = str[i];
        }
    }
    numeros[j] = '\0';
    return strtoul(numeros, NULL, 10);
}

// Extrai valor numérico de string tipo "R$ 5" ou "R$ 288.50"
float extrair_valor(const char *str) {
    const char *p = str;
    while (*p && (*p == 'R' || *p == '$' || *p == ' ')) p++;
    return strtof(p, NULL);
}

// Limpa o campo removendo aspas e espaços extras
void limpar_campo(char *dest, const char *src) {
    int i = 0, j = 0;
    while (src[i] == ' ') i++;
    while (src[i] != '\0' && src[i] != '\n') {
        if (src[i] != '"') {
            dest[j++] = src[i];
        }
        i++;
    }
    while (j > 0 && dest[j-1] == ' ') j--;
    dest[j] = '\0';
}

// Particiona linha por ';' e armazena em campos[] (máx de 16 campos)
int split_linha(const char *linha, char campos[][100], int max_campos) {
    int campo_idx = 0;
    int inicio = 0;
    int i;
    for (i = 0; linha[i] != '\0' && campo_idx < max_campos; i++) {
        if (linha[i] == ';') {
            int len = i - inicio;
            if (len >= 100) len = 99;
            strncpy(campos[campo_idx], linha + inicio, len);
            campos[campo_idx][len] = '\0';
            campo_idx++;
            inicio = i + 1;
        }
    }
    // último campo
    if (campo_idx < max_campos) {
        strncpy(campos[campo_idx], linha + inicio, max_campos - campo_idx);
        campos[campo_idx][99] = '\0';
        campo_idx++;
    }
    return campo_idx;
}

// Converte uma linha CSV em Transacao. Retorna 1 se válida, 0 se ignorar
int parse_linha(const char *linha, Transacao *t) {
    char campos[16][100];
    int total = split_linha(linha, campos, 16);
    
    // Percisa depelo menos 16 campos (índices 0 a 15)
    if (total < 16) return 0;
    
    // Campos:
    // 0: Transaction ID
    // 1: Date
    // 2: Day of Week (ignorado) NAO IMPORTA
    // 3: Time
    // 4: Type of Card
    // 5: Entry Mode (ignorado) NAO IMPORTA
    // 6: Amount
    // 7: Type of Transaction (ignorado) NAO IMPORTA
    // 8: Merchant Group
    // 9: Country of Transaction (ignorado) NAO IMPORTA
    // 10: Shipping Address (ignorado) NAO IMPORTA
    // 11: Country of Residence (ignorado) NAO IMPORTA
    // 12: Gender (ignorado) NAO IMPORTA
    // 13: Age (ignorado) NAO IMPORTA
    // 14: Bank (ignorado) NAO IMPORTA
    // 15: Fraud

    // ID
    t->id = extrair_id(campos[0]);
    
    // Data
    char data_convertida[11];
    converter_data(data_convertida, campos[1]);
    strncpy(t->data, data_convertida, 11);
    
    // Hora
    char hora_convertida[9];
    converter_hora(hora_convertida, campos[3]);
    strncpy(t->hora, hora_convertida, 9);
    
    // Bandeira (Type of Card)
    limpar_campo(t->bandeira, campos[4]);
    
    // Categoria (Merchant Group)
    limpar_campo(t->categoria, campos[8]);
    
    // Valor
    t->valor = extrair_valor(campos[6]);
    
    // Fraud (status)
    int fraud = atoi(campos[15]);
    t->status = (fraud == 0) ? 1 : 2; // 1 = Aprovada, 2 = Rejeitada
    
    // Validações adicionais pra id zero ou valor negativo, só pra garantir
    if (t->id == 0 || t->valor < 0.0) return 0;
    
    return 1;
}

int csv_para_binario(const char *csv_path, const char *bin_path) { // Aqui vai gerar o arquivo "transacoes.dat" em binario
    FILE *csv = fopen(csv_path, "r");
    if (!csv) {
        perror("Erro ao abrir CSV");
        return -1;
    }

    FILE *bin = fopen(bin_path, "wb");
    if (!bin) {
        perror("Erro ao criar arquivo binário");
        fclose(csv);
        return -1;
    }

    char linha[LINHA_MAX];
    int total = 0;
    int ignoradas = 0;
    Transacao t;

    // Pular cabeçalho (primeira linha)
    if (fgets(linha, LINHA_MAX, csv) == NULL) {
        fclose(csv);
        fclose(bin);
        return 0;
    }

    while (fgets(linha, LINHA_MAX, csv)) {
        // Remover quebra de linha
        linha[strcspn(linha, "\r\n")] = '\0';
        if (strlen(linha) == 0) {
            ignoradas++;
            continue;
        }

        if (parse_linha(linha, &t)) {
            fwrite(&t, sizeof(Transacao), 1, bin);
            total++;
        } else {
            ignoradas++;
        }
    }

    fclose(csv);
    fclose(bin);

    printf("Conversão concluída: %d transações salvas no binário (%d linhas ignoradas).\n", total, ignoradas);
    return total;
}