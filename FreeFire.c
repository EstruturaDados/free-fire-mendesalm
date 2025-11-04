/**
 * @file torre_de_fuga.c
 * @brief Desafio Mestre: Montagem da Torre de Fuga
 *
 * @details Este programa simula a montagem de uma torre de resgate
 * gerenciando 20 componentes críticos.
 *
 * Funcionalidades:
 * 1. Struct Componente (nome, tipo, prioridade 1-10).
 * 2. Cadastro de até 20 componentes.
 * 3. Três algoritmos de ordenação com propósitos distintos:
 * - Bubble Sort: Por Nome (alfabético).
 * - Insertion Sort: Por Tipo (alfabético).
 * - Selection Sort: Por Prioridade (decrescente, 10 -> 1).
 * 4. Busca Binária: Somente por nome, após a ordenação por nome.
 * 5. Análise de Desempenho (RF5):
 * - Contagem de comparações em cada ordenação.
 * - Medição de tempo de execução (usando <time.h>).
 * 6. Montagem Final (Listagem) e Busca por item-chave.
 */

// --- 1. Bibliotecas ---
#include <stdio.h>    // Para printf, scanf, fgets
#include <stdlib.h>   // Para system("cls")/"clear"
#include <string.h>   // Para strcmp, strcpy
#include <stdbool.h>  // Para bool, true, false
#include <time.h>     // RF5: Para clock() e medição de tempo

// --- 2. Definições e Structs Globais ---

/**
 * @brief RF2: Define o número máximo de componentes da torre.
 */
#define MAX_COMPONENTES 20

/**
 * @brief RF1: struct Componente
 * Armazena os dados de uma peça da torre.
 */
struct Componente {
    char nome[30];
    char tipo[20];
    int prioridade; // 1 (baixa) a 10 (altíssima)
};

// --- 3. Variáveis Globais (Estado da Torre) ---

/**
 * @brief O vetor principal que armazena os componentes.
 */
struct Componente torre[MAX_COMPONENTES];

/**
 * @brief Contador de quantos componentes estão cadastrados.
 */
int numComponentes = 0;

/**
 * @brief RF5: Contador para medição de desempenho de ordenação.
 * (Usamos long long para garantir que não estoure em N grande).
 */
long long comparacoes = 0;

/**
 * @brief RF4: Flag de segurança para a Busca Binária.
 * Só pode ser 'true' se a última ordenação foi por NOME.
 */
bool ordenadoPorNome = false;

// --- 4. Protótipos de Funções (Helpers e Menu) ---
void limparBufferEntrada();
void removerQuebraLinha(char* str);
void pausarTela();
void limparTela();
void exibirMenu();

// --- 5. Protótipos de Funções (Requisitos) ---
void inserirComponente();
void listarComponentes(); // RF6: Montagem Final
void menuDeOrdenacao();   // RF3, RF5
void bubbleSortPorNome();
void insertionSortPorTipo();
void selectionSortPorPrioridade();
void buscarComponenteChave(); // RF4

// --- 6. Função Principal (main) ---
int main() {
    int opcao = -1;

    do {
        limparTela();
        exibirMenu();
        printf("Escolha sua acao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                inserirComponente();
                pausarTela();
                break;
            case 2:
                // RF6: Exibe a "Montagem Final" no estado atual
                listarComponentes();
                pausarTela();
                break;
            case 3:
                // RF3, RF5: Entra no menu de estratégias de ordenação
                menuDeOrdenacao();
                pausarTela();
                break;
            case 4:
                // RF4: Tenta localizar o item-chave
                buscarComponenteChave();
                pausarTela();
                break;
            case 0:
                printf("\nSaindo da safe zone... Boa sorte.\n");
                break;
            default:
                printf("\nComando invalido!\n");
                pausarTela();
        }
    } while (opcao != 0);

    return 0;
}

// --- 7. Implementação das Funções de Menu e Helpers ---

void exibirMenu() {
    printf("--- Torre de Fuga (Safe Zone Final) ---\n");
    printf("Componentes Catalogados: %d/%d\n", numComponentes, MAX_COMPONENTES);
    printf("Status da Montagem: ");
    if (ordenadoPorNome) {
        printf("ORDENADA POR NOME (Busca Binaria ATIVA)\n");
    } else {
        printf("DESORDENADA (Busca Binaria INATIVA)\n");
    }
    printf("--------------------------------------------\n");
    printf("1. Inserir novo Componente\n");
    printf("2. Verificar Montagem (Listar Componentes)\n");
    printf("3. Executar Estrategia de Ordenacao\n");
    printf("4. Buscar Componente-Chave (Ativar Torre)\n");
    printf("0. Sair\n");
    printf("--------------------------------------------\n");
}

/**
 * @brief RF2: Adiciona um novo componente à torre.
 * Qualquer inserção invalida a ordenação por nome.
 */
void inserirComponente() {
    printf("\n[+] Inserindo Componente\n");
    if (numComponentes >= MAX_COMPONENTES) {
        printf("Erro: Limite de %d componentes atingido!\n", MAX_COMPONENTES);
        return;
    }

    struct Componente novo;
    printf("  > Nome (ex: Chip Central): ");
    fgets(novo.nome, 30, stdin);
    removerQuebraLinha(novo.nome);

    printf("  > Tipo (ex: Controle, Suporte): ");
    fgets(novo.tipo, 20, stdin);
    removerQuebraLinha(novo.tipo);

    printf("  > Prioridade (1-Baixa, 10-Alta): ");
    scanf("%d", &novo.prioridade);
    limparBufferEntrada();

    torre[numComponentes] = novo;
    numComponentes++;

    // Inserir um novo item quebra a ordem, desativa a busca binária.
    ordenadoPorNome = false;
    printf("Sucesso: Componente '%s' adicionado.\n", novo.nome);
}

/**
 * @brief RF6: Exibe a lista de componentes (a "montagem").
 * A exibição reflete a última ordenação executada.
 */
void listarComponentes() {
    printf("\n[#] Verificando Montagem da Torre (%d/%d)\n", numComponentes, MAX_COMPONENTES);
    if (numComponentes == 0) {
        printf("Nenhum componente catalogado.\n");
        return;
    }
    
    // Cabeçalho
    printf("| %-20s | %-15s | %-10s |\n", "Nome", "Tipo", "Prioridade");
    printf("|----------------------|-----------------|------------|\n");
    
    // Corpo
    for (int i = 0; i < numComponentes; i++) {
        printf("| %-20s | %-15s | %-10d |\n",
               torre[i].nome,
               torre[i].tipo,
               torre[i].prioridade);
    }
    printf("--------------------------------------------------\n");
}

/**
 * @brief RF3/RF5: Menu que permite escolher o algoritmo de ordenação
 * e mede o desempenho (tempo e comparações) de cada um.
 */
void menuDeOrdenacao() {
    printf("\n[#] Escolher Estrategia de Ordenacao\n");
    if (numComponentes < 2) {
        printf("Componentes insuficientes para ordenar.\n");
        return;
    }

    int escolha;
    printf("  1. Bubble Sort (Ordenar por NOME - A/Z)\n");
    printf("  2. Insertion Sort (Ordenar por TIPO - A/Z)\n");
    printf("  3. Selection Sort (Ordenar por PRIORIDADE - 10/1)\n");
    printf("  0. Cancelar\n");
    printf("Escolha: ");
    scanf("%d", &escolha);
    limparBufferEntrada();

    if (escolha == 0) return;

    // RF5: Prepara a medição de desempenho
    comparacoes = 0;         // Zera o contador de comparações
    clock_t inicio, fim;     // Variáveis para o tempo
    double tempoGasto;

    inicio = clock(); // Marca o início do tempo

    // Executa a ordenação escolhida
    switch (escolha) {
        case 1:
            printf("\nExecutando Bubble Sort por NOME...\n");
            bubbleSortPorNome();
            ordenadoPorNome = true; // RF4: ATIVA a busca binária
            break;
        case 2:
            printf("\nExecutando Insertion Sort por TIPO...\n");
            insertionSortPorTipo();
            ordenadoPorNome = false; // RF4: DESATIVA a busca binária
            break;
        case 3:
            printf("\nExecutando Selection Sort por PRIORIDADE...\n");
            selectionSortPorPrioridade();
            ordenadoPorNome = false; // RF4: DESATIVA a busca binária
            break;
        default:
            printf("Opcao invalida.\n");
            return;
    }

    fim = clock(); // Marca o fim do tempo

    // RF5: Calcula e exibe o desempenho
    tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\n--- Relatorio de Desempenho ---\n");
    printf("Ordenacao concluida.\n");
    printf("Total de Comparacoes: %lld\n", comparacoes);
    printf("Tempo de Execucao: %.10f segundos\n", tempoGasto);
    printf("---------------------------------\n");
    
    // Nota: Para N=20, o tempo será quase sempre 0.000000.
    // A medição de tempo é mais útil para N > 10.000.
}

// --- 8. Implementação dos Algoritmos de Ordenação (RF3) ---

/**
 * @brief RF3.1: Ordena por NOME usando Bubble Sort.
 * Otimizado com a flag 'trocou'.
 */
void bubbleSortPorNome() {
    bool trocou;
    for (int i = 0; i < numComponentes - 1; i++) {
        trocou = false;
        for (int j = 0; j < numComponentes - 1 - i; j++) {
            comparacoes++; // RF5: Conta a comparação
            // Se [j] vem *depois* de [j+1]
            if (strcmp(torre[j].nome, torre[j+1].nome) > 0) {
                // Troca
                struct Componente temp = torre[j];
                torre[j] = torre[j+1];
                torre[j+1] = temp;
                trocou = true;
            }
        }
        if (!trocou) break; // Vetor ordenado
    }
}

/**
 * @brief RF3.2: Ordena por TIPO usando Insertion Sort.
 */
void insertionSortPorTipo() {
    int i, j;
    struct Componente chave;
    for (i = 1; i < numComponentes; i++) {
        chave = torre[i];
        j = i - 1;

        // Move os elementos que são "maiores" que a chave
        // (alfabeticamente) uma posição para frente
        
        // Loop 'while' só continua se j>=0.
        // A comparação (RF5) é feita *dentro* do loop.
        while (j >= 0) {
            comparacoes++; // RF5: Conta a comparação
            if (strcmp(torre[j].tipo, chave.tipo) > 0) {
                torre[j + 1] = torre[j];
                j = j - 1;
            } else {
                break; // Encontrou a posição
            }
        }
        torre[j + 1] = chave;
    }
}

/**
 * @brief RF3.3: Ordena por PRIORIDADE (10 -> 1, decrescente)
 * usando Selection Sort.
 */
void selectionSortPorPrioridade() {
    int i, j, indiceMaior;
    for (i = 0; i < numComponentes - 1; i++) {
        // Encontra o índice do item de MAIOR prioridade
        indiceMaior = i;
        for (j = i + 1; j < numComponentes; j++) {
            comparacoes++; // RF5: Conta a comparação
            // Queremos a prioridade maior (ex: 10 > 9)
            if (torre[j].prioridade > torre[indiceMaior].prioridade) {
                indiceMaior = j;
            }
        }
        // Troca o item encontrado [indiceMaior] com o item [i]
        if (indiceMaior != i) {
            struct Componente temp = torre[i];
            torre[i] = torre[indiceMaior];
            torre[indiceMaior] = temp;
        }
    }
}


/**
 * @brief RF4: Busca Binária pelo componente-chave (por NOME).
 * Só funciona se 'ordenadoPorNome == true'.
 */
void buscarComponenteChave() {
    printf("\n[?] Ativando Torre (Busca Binaria)\n");

    // Trava de segurança
    if (!ordenadoPorNome) {
        printf("ALERTA: A montagem nao esta ordenada por NOME.\n");
        printf("Execute a Estrategia 3.1 (Bubble Sort por Nome) primeiro.\n");
        printf("Busca Binaria abortada por seguranca.\n");
        return;
    }
    if (numComponentes == 0) {
        printf("Nenhum componente na torre.\n");
        return;
    }
    
    char nomeBusca[30];
    printf("  > Digite o NOME do Componente-Chave para ativar: ");
    fgets(nomeBusca, 30, stdin);
    removerQuebraLinha(nomeBusca);

    // Lógica da Busca Binária
    int esquerda = 0;
    int direita = numComponentes - 1;
    bool encontrado = false;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        int cmp = strcmp(torre[meio].nome, nomeBusca);

        if (cmp == 0) {
            // RF6: Confirmar visualmente
            printf("\n==> COMPONENTE-CHAVE LOCALIZADO <==\n");
            printf("  Nome: %s\n", torre[meio].nome);
            printf("  Tipo: %s\n", torre[meio].tipo);
            printf("  Prioridade: %d\n", torre[meio].prioridade);
            printf("===================================\n");
            printf("TORRE ATIVADA!\n");
            encontrado = true;
            break;
        }
        if (cmp < 0) {
            esquerda = meio + 1; // Item está na metade direita
        } else {
            direita = meio - 1; // Item está na metade esquerda
        }
    }

    if (!encontrado) {
        printf("\n==> ALERTA: Componente-Chave '%s' nao encontrado <==\n", nomeBusca);
        printf("Ativacao da torre falhou!\n");
    }
}


// --- 9. Implementação das Funções Utilitárias ---

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removerQuebraLinha(char* str) {
    str[strcspn(str, "\n")] = 0;
}

void pausarTela() {
    printf("\nPressione ENTER para continuar...");
    limparBufferEntrada();
}

void limparTela() {
    // Usa 'system' para uma limpeza real
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
