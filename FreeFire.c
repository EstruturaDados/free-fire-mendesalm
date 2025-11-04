#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> // Para usar 'bool', 'true' e 'false'

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes
// coletados.
// Ele introduz ordenação com critérios e busca binária.

// --- Definições Globais ---

#define MAX_ITENS 10

/**
 * @brief Struct Item:
 * Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
 * A prioridade indica a importância do item na montagem do plano de fuga.
 */
struct Item {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade; // 1 (baixa) a 5 (altíssima)
};

/**
 * @brief Enum CriterioOrdenacao:
 * Define os critérios possíveis para a ordenação dos itens.
 */
enum CriterioOrdenacao {
    POR_NOME,
    POR_TIPO,
    POR_PRIORIDADE
};

// --- Variáveis Globais (Estado do Jogo) ---

/**
 * @brief Vetor mochila:
 * Armazena até 10 itens coletados.
 */
struct Item mochila[MAX_ITENS];

/**
 * @brief Variáveis de controle:
 * numItens: Quantidade atual de itens na mochila.
 * comparacoes: Contador para análise de desempenho da ordenação.
 * ordenadaPorNome: Flag essencial para permitir a busca binária.
 */
int numItens = 0;
int comparacoes = 0;
bool ordenadaPorNome = false;

// --- Protótipos das Funções ---

void limparTela();
void pausarTela(); // Utilitário para "Pressione Enter..."
void limparBufferEntrada();
void removerQuebraLinha(char* str);
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void insertionSort(enum CriterioOrdenacao criterio);
void buscaBinariaPorNome();

// --- Função Principal (main) ---

int main() {
    int opcao = -1;

    // A estrutura switch trata cada opção chamando a função correspondente.
    do {
        limparTela();
        exibirMenu(); // Mostra o menu principal
        
        printf("Escolha sua acao: ");
        scanf("%d", &opcao);
        limparBufferEntrada(); // Limpa o '\n' deixado pelo scanf

        switch (opcao) {
            case 1:
                inserirItem();
                pausarTela();
                break;
            case 2:
                removerItem();
                pausarTela();
                break;
            case 3:
                listarItens();
                pausarTela();
                break;
            case 4:
                menuDeOrdenacao();
                pausarTela();
                break;
            case 5:
                buscaBinariaPorNome();
                pausarTela();
                break;
            case 0:
                printf("\nSaindo da ilha... Boa sorte!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                pausarTela();
        }
        
    // A ordenação e busca binária exigem que os dados estejam bem organizados.
    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções Utilitárias ---

/**
 * @brief limparTela():
 * Simula a limpeza da tela imprimindo várias linhas em branco.
 * (Alternativa: system("cls") no Windows ou system("clear") no Linux/Mac)
 */
void limparTela() {
    // Usar system("cls") ou system("clear") é mais eficaz,
    // mas imprimir linhas funciona em qualquer terminal.
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

/**
 * @brief Pausa a execução até o usuário pressionar Enter.
 */
void pausarTela() {
    printf("\nPressione ENTER para continuar...");
    limparBufferEntrada(); // Aguarda o Enter
}

/**
 * @brief Limpa o buffer de entrada (stdin).
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Remove o caractere de quebra de linha ('\n') do final de
 * uma string lida por fgets().
 */
void removerQuebraLinha(char* str) {
    str[strcspn(str, "\n")] = 0;
}

// --- Implementação das Funções do Menu ---

/**
 * @brief exibirMenu():
 * Apresenta o menu principal ao jogador, com destaque para status da
 * ordenação.
 */
void exibirMenu() {
    printf("--- GERENCIADOR DE MOCHILA (Nivel Mestre) ---\n");
    printf("Itens na Mochila: %d/%d\n", numItens, MAX_ITENS);
    
    // Feedback crucial para o jogador
    if (ordenadaPorNome) {
        printf("Status: Mochila ORDENADA por nome. Busca Binaria ativada.\n");
    } else {
        printf("Status: Mochila DESORDENADA. Busca Binaria desativada.\n");
    }
    
    printf("-----------------------------------------------\n");
    printf("1. Adicionar um item\n");
    printf("2. Remover um item\n");
    printf("3. Listar todos os itens\n");
    printf("4. Ordenar os itens por criterio\n");
    printf("5. Realizar busca binaria por nome\n");
    printf("0. Sair\n");
    printf("-----------------------------------------------\n");
}

/**
 * @brief inserirItem():
 * Adiciona um novo componente à mochila se houver espaço.
 * Solicita nome, tipo, quantidade e prioridade.
 * Após inserir, marca a mochila como "não ordenada por nome".
 */
void inserirItem() {
    printf("\n[+] Adicionando novo item...\n");
    if (numItens >= MAX_ITENS) {
        printf("Erro: Mochila cheia!\n");
        return;
    }

    // Coleta dados
    struct Item novoItem;
    printf("    > Nome do item: ");
    fgets(novoItem.nome, 30, stdin);
    removerQuebraLinha(novoItem.nome);

    printf("    > Tipo do item (Arma, Cura, Municao): ");
    fgets(novoItem.tipo, 20, stdin);
    removerQuebraLinha(novoItem.tipo);

    printf("    > Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    
    printf("    > Prioridade (1-Baixa, 5-Alta): ");
    scanf("%d", &novoItem.prioridade);
    limparBufferEntrada(); // Limpa buffer após último scanf

    // Adiciona na próxima posição livre
    mochila[numItens] = novoItem;
    numItens++;

    // IMPORTANTE: Qualquer inserção pode quebrar a ordenação
    ordenadaPorNome = false;

    printf("Sucesso: Item '%s' adicionado.\n", novoItem.nome);
}

/**
 * @brief removerItem():
 * Permite remover um componente da mochila pelo nome.
 * Se encontrado, reorganiza o vetor para preencher a lacuna (shift left).
 */
void removerItem() {
    printf("\n[-] Removendo item...\n");
    if (numItens == 0) {
        printf("Erro: Mochila vazia!\n");
        return;
    }

    char nomeRemover[30];
    printf("    > Nome do item a remover: ");
    fgets(nomeRemover, 30, stdin);
    removerQuebraLinha(nomeRemover);

    int indiceEncontrado = -1;
    // Busca sequencial para encontrar o item
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Erro: Item '%s' nao encontrado.\n", nomeRemover);
        return;
    }

    // Reorganiza o vetor ("shift left")
    for (int i = indiceEncontrado; i < numItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    numItens--;

    // IMPORTANTE: A remoção também pode quebrar a ordenação
    // (Embora se remover o último, talvez não quebre, é mais seguro
    // invalidar a flag)
    ordenadaPorNome = false;

    printf("Sucesso: Item '%s' removido.\n", nomeRemover);
}

/**
 * @brief listarItens():
 * Exibe uma tabela formatada com todos os componentes presentes na mochila.
 */
void listarItens() {
    printf("\n--- Conteudo da Mochila (%d/%d) ---\n", numItens, MAX_ITENS);
    if (numItens == 0) {
        printf("A mochila esta vazia.\n");
        printf("--------------------------------------------------------------\n");
        return;
    }

    // Cabeçalho da tabela
    printf("| %-4s | %-20s | %-15s | %-5s | %-5s |\n", "Slot", "Nome", "Tipo", "Qtd", "Prio");
    printf("|------|----------------------|-----------------|-------|-------|\n");

    // Corpo da tabela
    for (int i = 0; i < numItens; i++) {
        printf("| %-4d | %-20s | %-15s | %-5d | %-5d |\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }
    printf("--------------------------------------------------------------\n");
}

/**
 * @brief menuDeOrdenacao():
 * Permite ao jogador escolher como deseja ordenar os itens.
 * Utiliza a função insertionSort() com o critério selecionado.
 * Exibe a quantidade de comparações feitas (análise de desempenho).
 */
void menuDeOrdenacao() {
    printf("\n[#] Ordenar Itens da Mochila\n");
    if (numItens < 2) {
        printf("Nao ha itens suficientes para ordenar.\n");
        return;
    }
    
    int criterio;
    printf("    1. Ordenar por Nome (A-Z)\n");
    printf("    2. Ordenar por Tipo (A-Z)\n");
    printf("    3. Ordenar por Prioridade (5-1)\n");
    printf("    Escolha o criterio: ");
    scanf("%d", &criterio);
    limparBufferEntrada();

    // Reseta o contador de comparações antes de ordenar
    comparacoes = 0;
    
    switch (criterio) {
        case 1:
            insertionSort(POR_NOME);
            ordenadaPorNome = true; // Flag ativada!
            printf("Mochila ordenada por NOME.\n");
            break;
        case 2:
            insertionSort(POR_TIPO);
            ordenadaPorNome = false; // Ordenou por tipo, não por nome
            printf("Mochila ordenada por TIPO.\n");
            break;
        case 3:
            insertionSort(POR_PRIORIDADE);
            ordenadaPorNome = false; // Ordenou por prioridade, não por nome
            printf("Mochila ordenada por PRIORIDADE.\n");
            break;
        default:
            printf("Criterio invalido.\n");
            return;
    }

    // Exibe a análise de desempenho
    printf("Ordenacao concluida. Total de comparacoes: %d\n", comparacoes);
}

/**
 * @brief insertionSort():
 * Implementação do algoritmo de ordenação por inserção (Insertion Sort).
 * Funciona com diferentes critérios de ordenação (nome, tipo, prioridade).
 * Atualiza a variável global 'comparacoes'.
 */
void insertionSort(enum CriterioOrdenacao criterio) {
    int i, j;
    struct Item chave; // O item que estamos tentando "inserir" no lugar certo

    // Começa do segundo elemento (índice 1)
    for (i = 1; i < numItens; i++) {
        chave = mochila[i];
        j = i - 1;

        bool deveTrocar = false;

        // Move os elementos da parte ordenada (mochila[0..i-1])
        // que são maiores (ou menores, dependendo do critério) que a chave
        // para uma posição à frente
        while (j >= 0) {
            comparacoes++; // Conta cada comparação dentro do loop
            
            // Lógica de comparação baseada no critério
            switch (criterio) {
                case POR_NOME:
                    // Se mochila[j] vem *depois* da chave (A-Z)
                    if (strcmp(mochila[j].nome, chave.nome) > 0) deveTrocar = true;
                    break;
                case POR_TIPO:
                    // Se mochila[j] vem *depois* da chave (A-Z)
                    if (strcmp(mochila[j].tipo, chave.tipo) > 0) deveTrocar = true;
                    break;
                case POR_PRIORIDADE:
                    // Se mochila[j] tem prioridade MENOR que a chave (5-1)
                    if (mochila[j].prioridade < chave.prioridade) deveTrocar = true;
                    break;
            }

            if (deveTrocar) {
                mochila[j + 1] = mochila[j]; // "Empurra" o item para a direita
                j = j - 1;
                deveTrocar = false; // Reseta para a próxima iteração do while
            } else {
                break; // Encontrou a posição correta, para o 'while'
            }
        }
        mochila[j + 1] = chave; // Insere a chave na posição correta
    }
}

/**
 * @brief buscaBinariaPorNome():
 * Realiza busca binária por nome, *somente* se a mochila estiver ordenada
 * por nome (flag ordenadaPorNome == true).
 */
void buscaBinariaPorNome() {
    printf("\n[?] Buscando item (Busca Binaria)...\n");

    // Requisito não funcional: A busca binária SÓ funciona em dados ordenados.
    if (!ordenadaPorNome) {
        printf("Erro: A mochila nao esta ordenada por nome.\n");
        printf("Por favor, use a Opcao 4 para ordenar por nome primeiro.\n");
        return;
    }

    if (numItens == 0) {
        printf("Erro: Mochila vazia!\n");
        return;
    }

    char nomeBusca[30];
    printf("    > Nome do item a buscar: ");
    fgets(nomeBusca, 30, stdin);
    removerQuebraLinha(nomeBusca);

    int esquerda = 0;
    int direita = numItens - 1;
    int comparacoesBusca = 0; // Contador local para esta busca
    bool encontrado = false;

    while (esquerda <= direita) {
        comparacoesBusca++; // Conta uma comparação
        int meio = esquerda + (direita - esquerda) / 2; // Evita overflow

        int cmp = strcmp(mochila[meio].nome, nomeBusca);

        if (cmp == 0) {
            // Encontrou!
            printf("\n==> Item Encontrado! <==\n");
            printf("    Slot: %d\n", meio + 1);
            printf("    Nome: %s\n", mochila[meio].nome);
            printf("    Tipo: %s\n", mochila[meio].tipo);
            printf("    Qtd:  %d\n", mochila[meio].quantidade);
            printf("    Prio: %d\n", mochila[meio].prioridade);
            encontrado = true;
            break;
        }

        if (cmp < 0) {
            // O item buscado está à direita (maior)
            esquerda = meio + 1;
        } else {
            // O item buscado está à esquerda (menor)
            direita = meio - 1;
        }
    }

    if (!encontrado) {
        printf("\n==> Item '%s' nao encontrado.\n", nomeBusca);
    }
    
    printf("Busca binaria realizou %d comparacoes.\n", comparacoesBusca);
}
