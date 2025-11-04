/**
 * @file mochila_comparativa.c
 * @brief Desafio: Comparação de Estruturas (Vetor vs. Lista Encadeada)
 *
 * @details Este programa implementa duas versões de um sistema de mochila
 * de jogo para comparar o desempenho de Listas Sequenciais (Vetores)
 * e Listas Encadeadas (Dinâmicas).
 *
 * Requisitos Implementados:
 * - RF1: struct Item (nome, tipo, qtd), struct No (Item, proximo).
 * - RF2: Implementações paralelas (Vetor e Lista).
 * - RF3: Operações (Inserir, Remover, Listar, Busca Sequencial) em ambas.
 * - RF4: Ordenação (Bubble Sort) no vetor.
 * - RF5: Busca Binária no vetor.
 * - RF6: Contadores de comparações em todas as buscas.
 * - RNF: Menu de usabilidade para escolher a estrutura.
 */

// --- 1. Bibliotecas Inclusas ---
#include <stdio.h>    // Para entrada e saída (printf, scanf, etc.)
#include <stdlib.h>   // Para alocação dinâmica (malloc, free)
#include <string.h>   // Para manipulação de strings (strcmp, strcpy)
#include <stdbool.h>  // Para usar 'bool', 'true', 'false'

// --- 2. Definições, Structs e Enums ---

/**
 * @brief Define a capacidade máxima de slots na mochila (Vetor).
 */
#define MAX_ITENS 10

/**
 * @brief RF1: struct Item
 * Armazena os dados de um item (sem prioridade).
 */
struct Item {
    char nome[30];
    char tipo[20];
    int quantidade;
};

/**
 * @brief RF1: struct No
 * Define o nó da lista encadeada.
 * Contém os 'dados' (o Item) e um ponteiro para o 'proximo' nó.
 */
struct No {
    struct Item dados;
    struct No* proximo;
};


// --- 3. Variáveis Globais (Estado do Jogo) ---

// --- Variáveis para a Mochila (Vetor) ---
struct Item mochilaVetor[MAX_ITENS];
int numItensVetor = 0;
bool ordenadoPorNomeVetor = false; // Flag para Busca Binária

// --- Variáveis para a Mochila (Lista Encadeada) ---
/**
 * @brief Ponteiro para a cabeça (início) da lista encadeada.
 * Se 'mochilaLista == NULL', a lista está vazia.
 */
struct No* mochilaLista = NULL;
int numItensLista = 0; // Útil para exibir status

// --- Variável Global de Desempenho ---
/**
 * @brief Contador global de comparações.
 * É zerado antes de cada operação de busca ou ordenação.
 */
int comparacoes = 0;


// --- 4. Protótipos das Funções Utilitárias ---
void limparTela();
void pausarTela();
void limparBufferEntrada();
void removerQuebraLinha(char* str);

// --- 5. Protótipos - Funções Principais (Menus) ---
void menuVetor();
void menuLista();

// --- 6. Protótipos - Funções da Mochila (VETOR) ---
void inserirItemVetor();
void removerItemVetor();
void listarItensVetor();
void buscarSequencialVetor();
void bubbleSortVetor();
void buscarBinariaVetor();

// --- 7. Protótipos - Funções da Mochila (LISTA ENCADEADA) ---
void inserirItemLista();
void removerItemLista();
void listarItensLista();
void buscarSequencialLista();


// --- 8. Função Principal (main) ---

/**
 * @brief Ponto de entrada. Apresenta o menu de seleção de estrutura.
 * @return 0 em caso de sucesso.
 */
int main() {
    int opcao = -1;

    do {
        limparTela();
        printf("--- Laboratorio de Comparacao: Mochila (Vetor vs. Lista) ---\n");
        printf("Qual estrutura voce deseja gerenciar?\n\n");
        printf("1. Mochila (VETOR / Lista Sequencial)\n");
        printf("   [+] Vantagem: Acesso rapido (Busca Binaria se ordenado).\n");
        printf("   [-] Desvantagem: Tamanho fixo, remocao lenta (shift).\n");
        printf("\n");
        printf("2. Mochila (LISTA ENCADEADA / Dinamica)\n");
        printf("   [+] Vantagem: Tamanho flexivel, insercao/remocao rapidas.\n");
        printf("   [-] Desvantagem: Lenta para buscar (Busca Sequencial apenas).\n");
        printf("\n");
        printf("0. Sair do Jogo\n");
        printf("----------------------------------------------------------\n");
        printf("Escolha sua plataforma de teste: ");
        
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                menuVetor(); // Entra no sub-menu do Vetor
                break;
            case 2:
                menuLista(); // Entra no sub-menu da Lista
                break;
            case 0:
                printf("\nSaindo do laboratorio... Resultados anotados!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                pausarTela();
        }

    } while (opcao != 0);

    // Limpeza de memória (libera todos os nós da lista encadeada)
    struct No* atual = mochilaLista;
    while (atual != NULL) {
        struct No* temp = atual;
        atual = atual->proximo;
        free(temp);
    }

    return 0;
}


// --- 9. Implementação dos Menus (Vetor e Lista) ---

/**
 * @brief Sub-menu para gerenciar a mochila baseada em VETOR.
 */
void menuVetor() {
    int opcao = -1;
    do {
        limparTela();
        printf("--- Gerenciando Mochila (VETOR) ---\n");
        printf("Itens: %d/%d | Ordenada por Nome: %s\n", 
               numItensVetor, MAX_ITENS, ordenadoPorNomeVetor ? "Sim" : "Nao");
        printf("--------------------------------------\n");
        printf("1. Inserir Item\n");
        printf("2. Remover Item (por nome)\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item (Sequencial)\n");
        printf("5. Ordenar Itens (Bubble Sort por Nome)\n");
        printf("6. Buscar Item (Binaria por Nome)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha sua acao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1: inserirItemVetor(); break;
            case 2: removerItemVetor(); break;
            case 3: listarItensVetor(); break;
            case 4: buscarSequencialVetor(); break;
            case 5: bubbleSortVetor(); break;
            case 6: buscarBinariaVetor(); break;
            case 0: break; // Sai do loop e volta para main
            default: printf("\nOpcao invalida!\n");
        }
        if (opcao != 0) pausarTela();

    } while (opcao != 0);
}

/**
 * @brief Sub-menu para gerenciar a mochila baseada em LISTA ENCADEADA.
 */
void menuLista() {
    int opcao = -1;
    do {
        limparTela();
        printf("--- Gerenciando Mochila (LISTA ENCADEADA) ---\n");
        printf("Itens: %d | (Tamanho dinamico)\n", numItensLista);
        printf("---------------------------------------------\n");
        printf("1. Inserir Item\n");
        printf("2. Remover Item (por nome)\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item (Sequencial)\n");
        printf("   (Obs: Ordenacao e Busca Binaria nao sao eficientes aqui.)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha sua acao: ");

        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1: inserirItemLista(); break;
            case 2: removerItemLista(); break;
            case 3: listarItensLista(); break;
            case 4: buscarSequencialLista(); break;
            case 0: break; // Sai do loop e volta para main
            default: printf("\nOpcao invalida!\n");
        }
        if (opcao != 0) pausarTela();

    } while (opcao != 0);
}

// --- 10. Implementação das Funções (VETOR) ---

void inserirItemVetor() {
    printf("\n[VETOR: Inserir Item]\n");
    if (numItensVetor >= MAX_ITENS) {
        printf("Erro: Mochila (Vetor) cheia!\n");
        return;
    }

    struct Item novoItem;
    printf("  > Nome: ");
    fgets(novoItem.nome, 30, stdin);
    removerQuebraLinha(novoItem.nome);
    printf("  > Tipo: ");
    fgets(novoItem.tipo, 20, stdin);
    removerQuebraLinha(novoItem.tipo);
    printf("  > Qtd: ");
    scanf("%d", &novoItem.quantidade);
    limparBufferEntrada();

    // Inserção O(1) no final da lista sequencial
    mochilaVetor[numItensVetor] = novoItem;
    numItensVetor++;

    // Qualquer inserção quebra a ordenação
    ordenadoPorNomeVetor = false;
    printf("Sucesso: Item '%s' adicionado ao vetor.\n", novoItem.nome);
}

void removerItemVetor() {
    printf("\n[VETOR: Remover Item]\n");
    if (numItensVetor == 0) {
        printf("Erro: Mochila (Vetor) vazia!\n");
        return;
    }

    char nomeRemover[30];
    printf("  > Nome do item a remover: ");
    fgets(nomeRemover, 30, stdin);
    removerQuebraLinha(nomeRemover);

    // 1. Busca Sequencial para encontrar o índice
    int indiceEncontrado = -1;
    for (int i = 0; i < numItensVetor; i++) {
        if (strcmp(mochilaVetor[i].nome, nomeRemover) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Erro: Item '%s' nao encontrado.\n", nomeRemover);
        return;
    }

    // 2. Remoção (Algoritmo "Shift Left") - Custo O(N)
    // Move todos os itens à direita uma posição para a esquerda
    for (int i = indiceEncontrado; i < numItensVetor - 1; i++) {
        mochilaVetor[i] = mochilaVetor[i + 1];
    }

    numItensVetor--;
    // A remoção pode quebrar a ordenação
    ordenadoPorNomeVetor = false;
    printf("Sucesso: Item '%s' removido do vetor.\n", nomeRemover);
}

void listarItensVetor() {
    printf("\n--- Conteudo da Mochila (VETOR) [%d/%d] ---\n", numItensVetor, MAX_ITENS);
    if (numItensVetor == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }
    printf("| %-4s | %-20s | %-15s | %-5s |\n", "Slot", "Nome", "Tipo", "Qtd");
    printf("|------|----------------------|-----------------|-------|\n");
    for (int i = 0; i < numItensVetor; i++) {
        printf("| %-4d | %-20s | %-15s | %-5d |\n",
               i + 1,
               mochilaVetor[i].nome,
               mochilaVetor[i].tipo,
               mochilaVetor[i].quantidade);
    }
    printf("--------------------------------------------------\n");
}

void buscarSequencialVetor() {
    printf("\n[VETOR: Busca Sequencial]\n");
    if (numItensVetor == 0) {
        printf("Mochila vazia!\n");
        return;
    }

    char nomeBusca[30];
    printf("  > Nome do item a buscar: ");
    fgets(nomeBusca, 30, stdin);
    removerQuebraLinha(nomeBusca);

    comparacoes = 0; // RF6: Zera o contador
    bool encontrado = false;

    // Algoritmo de Busca Sequencial O(N)
    for (int i = 0; i < numItensVetor; i++) {
        comparacoes++; // Conta uma comparação de string
        if (strcmp(mochilaVetor[i].nome, nomeBusca) == 0) {
            printf("\n==> Item Encontrado! <==\n");
            printf("    Slot: %d\n", i + 1);
            printf("    Nome: %s | Tipo: %s | Qtd: %d\n",
                   mochilaVetor[i].nome, mochilaVetor[i].tipo, mochilaVetor[i].quantidade);
            encontrado = true;
            break; // Para a busca
        }
    }

    if (!encontrado) {
        printf("\n==> Item '%s' nao encontrado.\n", nomeBusca);
    }
    printf("Total de comparacoes (Sequencial Vetor): %d\n", comparacoes);
}

/**
 * @brief RF4: Implementa o Bubble Sort para ordenar o vetor por nome.
 */
void bubbleSortVetor() {
    printf("\n[VETOR: Ordenando (Bubble Sort)]\n");
    if (numItensVetor < 2) {
        printf("Nao ha itens suficientes para ordenar.\n");
        return;
    }

    comparacoes = 0; // RF6: Zera o contador
    bool trocou;

    // Algoritmo Bubble Sort O(N^2)
    for (int i = 0; i < numItensVetor - 1; i++) {
        trocou = false;
        // O loop interno "borbulha" o item "maior" (alfabeticamente)
        for (int j = 0; j < numItensVetor - 1 - i; j++) {
            comparacoes++; // Conta uma comparação
            // Se o item [j] vem *depois* do [j+1]
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[j+1].nome) > 0) {
                // Troca
                struct Item temp = mochilaVetor[j];
                mochilaVetor[j] = mochilaVetor[j+1];
                mochilaVetor[j+1] = temp;
                trocou = true;
            }
        }
        // Se não houve trocas nesta passada, o vetor está ordenado
        if (!trocou) break;
    }

    ordenadoPorNomeVetor = true; // RF5: Habilita a busca binária
    printf("Mochila (Vetor) ordenada por nome.\n");
    printf("Total de comparacoes (Bubble Sort): %d\n", comparacoes);
}

void buscarBinariaVetor() {
    printf("\n[VETOR: Busca Binaria]\n");
    
    // RF5: Pré-requisito
    if (!ordenadoPorNomeVetor) {
        printf("Erro: A mochila (Vetor) nao esta ordenada por nome.\n");
        printf("Por favor, use a Opcao 5 para ordenar primeiro.\n");
        return;
    }
    if (numItensVetor == 0) {
        printf("Mochila vazia!\n");
        return;
    }

    char nomeBusca[30];
    printf("  > Nome do item a buscar: ");
    fgets(nomeBusca, 30, stdin);
    removerQuebraLinha(nomeBusca);

    comparacoes = 0; // RF6: Zera o contador
    bool encontrado = false;
    int esquerda = 0;
    int direita = numItensVetor - 1;

    // Algoritmo de Busca Binária O(log N)
    while (esquerda <= direita) {
        comparacoes++; // Conta uma tentativa/comparação
        int meio = esquerda + (direita - esquerda) / 2;
        int cmp = strcmp(mochilaVetor[meio].nome, nomeBusca);

        if (cmp == 0) {
            // Encontrou!
            printf("\n==> Item Encontrado! <==\n");
            printf("    Slot: %d\n", meio + 1);
            printf("    Nome: %s | Tipo: %s | Qtd: %d\n",
                   mochilaVetor[meio].nome, mochilaVetor[meio].tipo, mochilaVetor[meio].quantidade);
            encontrado = true;
            break;
        } else if (cmp < 0) {
            // O item do 'meio' é "menor" que o buscado. Ignora metade esquerda.
            esquerda = meio + 1;
        } else {
            // O item do 'meio' é "maior" que o buscado. Ignora metade direita.
            direita = meio - 1;
        }
    }

    if (!encontrado) {
        printf("\n==> Item '%s' nao encontrado.\n", nomeBusca);
    }
    printf("Total de comparacoes (Binaria Vetor): %d\n", comparacoes);
}


// --- 11. Implementação das Funções (LISTA ENCADEADA) ---

void inserirItemLista() {
    printf("\n[LISTA: Inserir Item]\n");

    // 1. Coletar dados
    struct Item novoItem;
    printf("  > Nome: ");
    fgets(novoItem.nome, 30, stdin);
    removerQuebraLinha(novoItem.nome);
    printf("  > Tipo: ");
    fgets(novoItem.tipo, 20, stdin);
    removerQuebraLinha(novoItem.tipo);
    printf("  > Qtd: ");
    scanf("%d", &novoItem.quantidade);
    limparBufferEntrada();

    // 2. Alocar memória para o novo nó
    struct No* novoNo = (struct No*)malloc(sizeof(struct No));
    if (novoNo == NULL) {
        printf("Erro: Falha ao alocar memoria (malloc)!\n");
        return;
    }

    // 3. Preencher o novo nó
    novoNo->dados = novoItem;

    // 4. Inserir no INÍCIO da lista - Custo O(1)
    // O novo nó aponta para a antiga "cabeça"
    novoNo->proximo = mochilaLista; 
    // A "cabeça" da lista agora é o novo nó
    mochilaLista = novoNo; 

    numItensLista++;
    printf("Sucesso: Item '%s' adicionado a lista.\n", novoItem.nome);
}

void removerItemLista() {
    printf("\n[LISTA: Remover Item]\n");
    if (mochilaLista == NULL) { // Lista vazia
        printf("Erro: Mochila (Lista) vazia!\n");
        return;
    }

    char nomeRemover[30];
    printf("  > Nome do item a remover: ");
    fgets(nomeRemover, 30, stdin);
    removerQuebraLinha(nomeRemover);

    struct No* atual = mochilaLista;
    struct No* anterior = NULL;

    // 1. Busca Sequencial para encontrar o nó - Custo O(N)
    while (atual != NULL && strcmp(atual->dados.nome, nomeRemover) != 0) {
        anterior = atual;     // O anterior "segue" o atual
        atual = atual->proximo; // O atual avança
    }

    // 2. Verificar se encontrou
    if (atual == NULL) { // Chegou ao fim da lista e não achou
        printf("Erro: Item '%s' nao encontrado.\n", nomeRemover);
        return;
    }

    // 3. Remover o nó (Custo O(1) após a busca)
    if (anterior == NULL) {
        // Caso 1: O item a remover é a CABEÇA da lista
        mochilaLista = atual->proximo; // A cabeça agora é o próximo
    } else {
        // Caso 2: O item está no meio ou fim
        // O anterior "pula" o 'atual' e aponta para o próximo
        anterior->proximo = atual->proximo; 
    }

    // 4. Liberar a memória do nó removido
    free(atual); 
    numItensLista--;
    printf("Sucesso: Item '%s' removido da lista.\n", nomeRemover);
}

void listarItensLista() {
    printf("\n--- Conteudo da Mochila (LISTA) [%d Itens] ---\n", numItensLista);
    if (mochilaLista == NULL) {
        printf("A mochila esta vazia.\n");
        return;
    }

    printf("| %-4s | %-20s | %-15s | %-5s |\n", "Slot", "Nome", "Tipo", "Qtd");
    printf("|------|----------------------|-----------------|-------|\n");
    
    struct No* atual = mochilaLista; // Começa na cabeça
    int i = 1;

    // Loop O(N) para percorrer a lista
    while (atual != NULL) {
        printf("| %-4d | %-20s | %-15s | %-5d |\n",
               i,
               atual->dados.nome,
               atual->dados.tipo,
               atual->dados.quantidade);
        atual = atual->proximo; // Avança para o próximo nó
        i++;
    }
    printf("--------------------------------------------------\n");
}

void buscarSequencialLista() {
    printf("\n[LISTA: Busca Sequencial]\n");
    if (mochilaLista == NULL) {
        printf("Mochila vazia!\n");
        return;
    }

    char nomeBusca[30];
    printf("  > Nome do item a buscar: ");
    fgets(nomeBusca, 30, stdin);
    removerQuebraLinha(nomeBusca);

    comparacoes = 0; // RF6: Zera o contador
    bool encontrado = false;
    struct No* atual = mochilaLista;
    int i = 1;

    // Algoritmo de Busca Sequencial O(N)
    while (atual != NULL) {
        comparacoes++; // Conta uma comparação de string
        if (strcmp(atual->dados.nome, nomeBusca) == 0) {
            printf("\n==> Item Encontrado! <==\n");
            printf("    Slot: %d\n", i);
            printf("    Nome: %s | Tipo: %s | Qtd: %d\n",
                   atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
            encontrado = true;
            break; // Para a busca
        }
        atual = atual->proximo; // Avança
        i++;
    }

    if (!encontrado) {
        printf("\n==> Item '%s' nao encontrado.\n", nomeBusca);
    }
    printf("Total de comparacoes (Sequencial Lista): %d\n", comparacoes);
}


// --- 12. Implementação das Funções Utilitárias ---

void limparTela() {
    // A forma mais portável de "limpar"
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void pausarTela() {
    printf("\nPressione ENTER para continuar...");
    limparBufferEntrada();
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removerQuebraLinha(char* str) {
    str[strcspn(str, "\n")] = 0;
}
