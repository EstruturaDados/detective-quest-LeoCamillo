#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 🌲 Definição da Estrutura da Sala (Nó da Árvore)

/**
 * @brief Representa um cômodo da mansão, que é um nó na árvore binária.
 * * Cada sala tem um nome e ponteiros para os cômodos à esquerda e à direita.
 */
typedef struct Sala {
    char nome[50];       // Nome do cômodo
    struct Sala *esquerda; // Ponteiro para o cômodo da esquerda
    struct Sala *direita;  // Ponteiro para o cômodo da direita
} Sala;


// 🛠️ Funções de Criação e Manipulação

/**
 * @brief Cria dinamicamente um novo cômodo (nó) com o nome especificado.
 * * Utiliza malloc para alocar memória e inicializa o nome e os ponteiros como NULL.
 * @param nome O nome do novo cômodo.
 * @return Um ponteiro para a Sala recém-criada, ou NULL se a alocação falhar.
 */
Sala* criarSala(const char *nome) {
    // 1. Aloca memória dinamicamente para a nova sala.
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    
    // 2. Verifica se a alocação foi bem-sucedida.
    if (novaSala == NULL) {
        printf("Erro de alocação de memória.\n");
        return NULL;
    }
    
    // 3. Inicializa os campos da nova sala.
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0'; // Garantir terminação de string
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * @brief Permite ao jogador navegar interativamente pela mansão (árvore binária).
 * * O jogador começa no nó raiz e escolhe 'e' (esquerda), 'd' (direita) ou 's' (sair).
 * A exploração termina ao chegar a um cômodo sem caminhos (folha) ou ao escolher 's'.
 * @param salaAtual O ponteiro para o cômodo atual (inicia no Hall de Entrada).
 */
void explorarSalas(Sala *salaAtual) {
    char escolha;
    
    printf("\n--- Início da Exploração ---\n");
    
    // O loop continua enquanto o jogador não chegar a uma sala sem saída
    // (um nó-folha) e não escolher 's' para sair.
    while (salaAtual != NULL) {
        // 1. Exibe o cômodo atual e opções.
        printf("\nVocê está em: **%s**\n", salaAtual->nome);
        
        // Verifica se é uma sala sem saída (nó folha).
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("⚠️ Este cômodo não tem mais saídas. Sua exploração termina aqui.\n");
            break; // Sai do loop.
        }
        
        // 2. Exibe as opções de navegação.
        printf("Escolha seu caminho:\n");
        
        if (salaAtual->esquerda != NULL) {
            printf("  [e] Ir para a Esquerda\n");
        }
        if (salaAtual->direita != NULL) {
            printf("  [d] Ir para a Direita\n");
        }
        printf("  [s] Sair da exploração\n");
        printf("Sua escolha (e/d/s): ");
        
        // 3. Lê a escolha do jogador.
        if (scanf(" %c", &escolha) != 1) {
            // Caso de erro na leitura
            while(getchar() != '\n'); // Limpa o buffer de entrada
            escolha = ' ';
        } else {
             // Converte para minúscula para simplificar a comparação
            escolha = tolower(escolha);
        }

        // 4. Processa a escolha do jogador.
        if (escolha == 's') {
            printf("\nVocê optou por **sair** da exploração. Até mais!\n");
            break;
        } else if (escolha == 'e') {
            if (salaAtual->esquerda != NULL) {
                salaAtual = salaAtual->esquerda;
            } else {
                printf("❌ Não há caminho para a Esquerda. Tente outra opção.\n");
            }
        } else if (escolha == 'd') {
            if (salaAtual->direita != NULL) {
                salaAtual = salaAtual->direita;
            } else {
                printf("❌ Não há caminho para a Direita. Tente outra opção.\n");
            }
        } else {
            printf("⁉️ Opção inválida. Digite 'e', 'd' ou 's'.\n");
        }
        
        // Garante que o buffer de entrada seja limpo após a leitura de char
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    printf("\n--- Fim da Exploração ---\n");
}

/**
 * @brief Libera a memória de toda a árvore de cômodos (percorrimento pós-ordem).
 * @param sala O ponteiro para o nó a ser liberado.
 */
void liberarMapa(Sala *sala) {
    if (sala != NULL) {
        liberarMapa(sala->esquerda);
        liberarMapa(sala->direita);
        free(sala); // Libera o próprio nó após liberar seus filhos
    }
}


// 🚀 Função Principal
int main() {
    printf("--- Detective Quest: Mapa da Mansão ---\n");
    printf("Bem-vindo(a) à exploração da mansão. O mapa está sendo montado...\n");

    // 1. Criação do Mapa da Mansão (Árvore Binária)

    // Nível 0 (Raiz)
    Sala *hallDeEntrada = criarSala("Hall de Entrada");
    if (hallDeEntrada == NULL) return 1; // Sair em caso de falha na alocação

    // Nível 1
    hallDeEntrada->esquerda = criarSala("Sala de Estar");
    hallDeEntrada->direita = criarSala("Cozinha");

    // Nível 2
    if (hallDeEntrada->esquerda != NULL) {
        hallDeEntrada->esquerda->esquerda = criarSala("Jardim de Inverno"); // Folha
        hallDeEntrada->esquerda->direita = criarSala("Escritório");
    }
    if (hallDeEntrada->direita != NULL) {
        hallDeEntrada->direita->esquerda = criarSala("Sala de Jantar");
        // Direita da Cozinha fica NULL, não há caminho.
    }

    // Nível 3
    if (hallDeEntrada->esquerda != NULL && hallDeEntrada->esquerda->direita != NULL) {
        hallDeEntrada->esquerda->direita->esquerda = criarSala("Quarto Principal");
        hallDeEntrada->esquerda->direita->direita = criarSala("Biblioteca"); // Folha
    }
    if (hallDeEntrada->direita != NULL && hallDeEntrada->direita->esquerda != NULL) {
        hallDeEntrada->direita->esquerda->esquerda = criarSala("Despensa"); // Folha
        hallDeEntrada->direita->esquerda->direita = criarSala("Porão"); // Folha
    }
    
    // Nível 4 (Folha, ramificação do Quarto Principal)
    if (hallDeEntrada->esquerda != NULL && hallDeEntrada->esquerda->direita != NULL && 
        hallDeEntrada->esquerda->direita->esquerda != NULL) {
        // Direita do Quarto Principal fica NULL
        hallDeEntrada->esquerda->direita->esquerda->esquerda = criarSala("Banheiro"); // Folha
    }

    // 2. Início da Exploração
    explorarSalas(hallDeEntrada);

    // 3. Limpeza de Memória
    liberarMapa(hallDeEntrada);
    printf("\nMemória do mapa liberada. O programa terminou.\n");

    return 0;
}