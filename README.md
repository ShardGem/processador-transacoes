# Motor de Processamento de Transações

Um sistema em C que simula o backend de uma operadora de cartões (tipo Stone, Cielo, PagSeguro). A ideia era de construir do zero um pipeline que consegue ler um CSV gigante, transforma os dados em binário para perfomance, ordena e distribui tudo em fila, pilha e lista encadeada, liberando a memória corretamente no final.

---

## O que esse programa faz?

1. **Lê** o arquivo `CreditCardData.csv` (que tem 100 mil transações) e faz uma limpeza e tratamento nos dados.
2. **Converte** cada transação válida para um formato binário indexado (`transacoes.dat`), simulando um banco de dados rápido.
3. **Ordena** o vetor de transações por data/hora (ordem cronológica) usando `qsort`.
4. **Distribui** as transações:
   - **Fila**: autorização de crédito (FIFO) para transações sem fraude.
   - **Pilha**: rejeições (LIFO) com função de "desfazer" (undo).
   - **Lista encadeada dinâmica**: transações de risco (valor alto durante a madrugada).
5. **Gera** o relatório `relatorio_consolidado.txt` com métricas de desempenho, valores totais por bandeira, maior transação e a lista de suspeitas.
6. **Libera** toda a memória alocada antes de fechar (morra vazamentos de memória).

---

## Estrutura do Projeto

processador-transacoes/

│

├── transacao.h # Struct base das transações

├── conversor.h / .c # Leitura do CSV e conversão para binário

├── ordenacao.h / .c # Ordenação por valor e data/hora

├── fila.h / .c # Fila (FIFO) para autorização

├── pilha.h / .c # Pilha (LIFO) para rejeições/estorno

├── lista_risco.h / .c # Lista encadeada de transações suspeitas

├── relatorio.h / .c # Geração do relatório final

├── main.c # Orquestra todo o pipeline

├── CreditCardData.csv # Base de dados (não versionada, se preferir)

└── README.md

---

## Como compilar e rodar
Você precisa do **gcc** e um terminal. Qualquer Linux ou Codespaces serve.

```bash
# Compila tudo
gcc -Wall -std=c99 -o processador *.c

# Executa
./processador

tmj pprt
