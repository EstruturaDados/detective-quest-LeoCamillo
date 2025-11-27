#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- ESTRUTURAS DE DADOS ---

// 1. 🌲 Estrutura para a Árvore de Pistas (BST)
/**
 * @brief Nó da Árvore Binária de Busca (BST) que armazena as pistas coletadas.
 * A ordem é definida pelo conteúdo da pista (alfabética).
 */
typedef struct PistaNode {
    char pista[100];         // Conteúdo da pista
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;


// 2. 🏠 Estrutura para a Sala da Mansão (Árvore Binária)
/**
 * @brief Nó do mapa da mansão. Representa um cômodo e pode conter uma pista.
 */
typedef struct Sala {
    char nome[50];
    char pista[100];        // Pista associada a este cômodo (pode ser vazia "")
    struct Sala *esquerda;  // Caminho 'e'
    struct Sala *direita;   // Caminho 'd'
} Sala;


// --- FUNÇÕES DE MANIPULAÇÃO DA BST (PISTAS) ---

/**
 * @brief Cria dinamicamente um novo nó de pista (PistaNode).
 * @param pista O conteúdo da pista a ser armazenada.
 * @return Um ponteiro para o novo PistaNode.
 */
PistaNode* criarPistaNode(const char *pista) {
    PistaNode *novo = (PistaNode *)malloc(sizeof(PistaNode));
    if (novo == NULL) {
        perror("Erro de alocação de memória para PistaNode");
        exit(EXIT_FAILURE);
    }
    strncpy(novo->pista, pista, sizeof(novo->pista) - 1);
    novo->pista[sizeof(novo->pista) - 1] = '\0';
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

/**
 * @brief Insere uma nova pista na Árvore Binária de Busca (BST).
 * @param raiz O ponteiro para a raiz da BST.
 * @param pista O conteúdo da pista a ser inserida.
 * @return O ponteiro para a raiz da BST atualizada.
 */
PistaNode* inserirPista(PistaNode* raiz, const char *pista) {
    // Caso base: Se a árvore estiver vazia, retorna um novo nó.
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }

    // Compara a nova pista com a pista do nó atual.
    int comparacao = strcmp(pista, raiz->pista);

    if (comparacao < 0) {
        // Se a nova pista for alfabeticamente menor, vai para a subárvore esquerda.
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        // Se a nova pista for alfabeticamente maior, vai para a subárvore direita.
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se comparacao == 0, a pista já existe (não faz nada).

    return raiz;
}

/**
 * @brief Percorre a BST em ordem (In-Order) para exibir as pistas coletadas em ordem alfabética.
 * @param raiz O ponteiro para a raiz da BST.
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/**
 * @brief Libera a memória de todos os nós da BST.
 * @param raiz O ponteiro para a raiz da BST.
 */
void liberarPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}


// --- FUNÇÕES DE MANIPULAÇÃO DO MAPA (MANSÃO) ---

/**
 * @brief Cria dinamicamente um novo cômodo (Sala) com o nome e a pista especificados.
 * @param nome O nome do cômodo.
 * @param pista O conteúdo da pista (use "" se não houver pista).
 * @return Um ponteiro para a Sala recém-criada.
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        perror("Erro de alocação de memória para Sala");
        exit(EXIT_FAILURE);
    }
    
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0';
    
    strncpy(novaSala->pista, pista, sizeof(novaSala->pista) - 1);
    novaSala->pista[sizeof(novaSala->pista) - 1] = '\0';
    
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * @brief Libera a memória de toda a árvore de cômodos (mapa).
 * @param sala O ponteiro para o nó a ser liberado.
 */
void liberarMapa(Sala *sala) {
    if (sala != NULL) {
        liberarMapa(sala->esquerda);
        liberarMapa(sala->direita);
        free(sala);
    }
}


// --- FUNÇÃO DE EXPLORAÇÃO PRINCIPAL ---

/**
 * @brief Controla a navegação do jogador pela mansão, exibindo salas e coletando pistas.
 * @param salaAtual O ponteiro para o cômodo atual (inicia no Hall de Entrada).
 * @param bstPistas O ponteiro para a raiz da BST de pistas coletadas.
 * @return O ponteiro atualizado para a raiz da BST de pistas.
 */
PistaNode* explorarSalasComPistas(Sala *hallDeEntrada, PistaNode *bstPistas) {
    Sala *salaAtual = hallDeEntrada;
    char escolha;
    
    printf("\n--- Início da Exploração e Coleta de Pistas ---\n");
    
    while (salaAtual != NULL) {
        
        printf("\nVocê está em: **%s**\n", salaAtual->nome);
        
        // 1. Verifica e Coleta a Pista
        if (strlen(salaAtual->pista) > 0) {
            printf("🔍 **PISTA ENCONTRADA!** Conteúdo: \"%s\"\n", salaAtual->pista);
            
            // Insere a pista na BST. A função BST cuida de duplicatas.
            bstPistas = inserirPista(bstPistas, salaAtual->pista);
            
            // Limpa a pista da sala para que não seja coletada novamente
            salaAtual->pista[0] = '\0'; 
        } else {
            printf("  (Nenhuma pista nova neste cômodo.)\n");
        }
        
        // 2. Exibe as opções de navegação.
        printf("\nOpções de caminho:\n");
        
        if (salaAtual->esquerda != NULL) {
            printf("  [e] Ir para a Esquerda (%s)\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf("  [d] Ir para a Direita (%s)\n", salaAtual->direita->nome);
        }
        printf("  [s] Sair da exploração e ver as pistas coletadas\n");
        printf("Sua escolha (e/d/s): ");
        
        // 3. Lê e processa a escolha do jogador.
        if (scanf(" %c", &escolha) != 1) {
            while(getchar() != '\n'); 
            escolha = ' ';
        } else {
            escolha = tolower(escolha);
        }

        if (escolha == 's') {
            printf("\nVocê optou por **sair** da exploração.\n");
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
        
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    return bstPistas;
}

// 🚀 Função Principal
int main() {
    
    // Inicializa a raiz da BST de pistas como NULL
    PistaNode *bstPistas = NULL;
    Sala *hallDeEntrada = NULL;

    printf("--- Detective Quest: Nível Aventureiro ---\n");
    printf("Montando o mapa da mansão e espalhando as pistas...\n");

    // 1. Criação do Mapa da Mansão (Árvore Binária com Pistas)
    // Pista: "" (string vazia) significa que não há pista no cômodo.
    
    // Nível 0 (Raiz)
    hallDeEntrada = criarSala("Hall de Entrada", "A chave está sob o primeiro objeto pesado.");

    // Nível 1
    hallDeEntrada->esquerda = criarSala("Sala de Estar", "A vela queimada esconde um número.");
    hallDeEntrada->direita = criarSala("Cozinha", "");

    // Nível 2
    if (hallDeEntrada->esquerda) {
        hallDeEntrada->esquerda->esquerda = criarSala("Jardim de Inverno", "A resposta é a capital do Chile."); 
        hallDeEntrada->esquerda->direita = criarSala("Escritório", "");
    }
    if (hallDeEntrada->direita) {
        hallDeEntrada->direita->esquerda = criarSala("Sala de Jantar", "O culpado usa um chapéu vermelho.");
    }

    // Nível 3
    if (hallDeEntrada->esquerda && hallDeEntrada->esquerda->direita) {
        hallDeEntrada->esquerda->direita->esquerda = criarSala("Quarto Principal", "O relógio parou às 04:15.");
        hallDeEntrada->esquerda->direita->direita = criarSala("Biblioteca", "");
    }
    if (hallDeEntrada->direita && hallDeEntrada->direita->esquerda) {
        hallDeEntrada->direita->esquerda->esquerda = criarSala("Despensa", "O livro preferido é 'Crimes do Nilo'.");
        hallDeEntrada->direita->esquerda->direita = criarSala("Porão", "Existe uma segunda porta secreta.");
    }
    
    // Nível 4
    if (hallDeEntrada->esquerda && hallDeEntrada->esquerda->direita && hallDeEntrada->esquerda->direita->esquerda) {
        hallDeEntrada->esquerda->direita->esquerda->esquerda = criarSala("Banheiro", "");
    }

    // 2. Início da Exploração
    bstPistas = explorarSalasComPistas(hallDeEntrada, bstPistas);

    // 3. Exibição Organizada das Pistas Coletadas
    printf("\n============================================\n");
    printf("✅ Pistas Coletadas (Em Ordem Alfabética):\n");
    printf("============================================\n");
    if (bstPistas == NULL) {
        printf("Nenhuma pista foi coletada durante a exploração.\n");
    } else {
        exibirPistas(bstPistas); // Usa o percorrimento In-Order da BST
    }
    printf("============================================\n");

    // 4. Limpeza de Memória
    liberarMapa(hallDeEntrada);
    liberarPistas(bstPistas);
    printf("\nMemória do mapa e das pistas liberada. Programa encerrado.\n");

    return 0;
}