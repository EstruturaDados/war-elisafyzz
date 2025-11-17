#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes ---
#define MAX_NOME 30
#define MAX_COR 10
#define TAM_MISSAO 100
#define NUM_MISSOES 5

// --- 1. Criacao da Struct Territorio ---
/**
 * @struct Territorio
 * @brief Estrutura basica para guardar informacoes de um territorio.
 * Campos: nome, cor do exercito (dono) e tropas.
 */
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// --- Vetor de Missoes (Requisito) ---
// Vetor de strings contendo as missoes estrategicas pre-definidas.
const char* missoesEstrategicas[NUM_MISSOES] = {
    "MISS1: Conquistar todos os territorios da cor AZUL.",
    "MISS2: Eliminar todas as tropas da cor VERMELHA.",
    "MISS3: Conquistar 3 territorios com mais de 5 tropas.",
    "MISS4: Ter pelo menos 10 tropas no territorio 'Territorio A'.",
    "MISS5: Conquistar 4 territorios seguidos na lista."
};

// --- Prototipos das Funcoes Modulares (Requisito) ---
// Funcoes de Cadastro e Exibicao
void cadastrarTerritorios(Territorio* mapa, int tamanho);
void exibirMapa(const Territorio* mapa, int tamanho);

// Funcoes de Batalha
void atacar(Territorio* atacante, Territorio* defensor);

// Funcoes de Missao
void atribuirMissao(char** missaoDestino, const char* missoes[], int totalMissoes);
void exibirMissao(const char* missao);
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho);

// Funcao de Gerenciamento de Memoria
void liberarMemoria(Territorio* mapa, char* missao);

// --- Funcao Principal ---
int main() {
    // Requisito Tecnico: Inicializa o gerador de numeros aleatorios
    srand(time(NULL));
    
    int numTerritorios = 0;
    Territorio* mapa = NULL; 
    char* missaoJogador = NULL; 

    printf("### Jogo de Territorios Completo ###\n");
    
    // --- Alocacao Dinamica de Territorios (Requisito) ---
    printf("Digite o numero total de territorios para o mapa (Recomendado: 5): ");
    if (scanf("%d", &numTerritorios) != 1 || numTerritorios <= 0) {
        printf("Erro: Numero invalido de territorios. Encerrando.\n");
        return 1;
    }
    
    // Aloca a memoria para o vetor de structs
    mapa = (Territorio*)calloc(numTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro: Falha ao alocar memoria para o mapa.\n");
        return 1;
    }

    // --- Cadastro Inicial (Requisito) ---
    // O sistema permite cadastro. O tamanho e dinamico, mas se o usuario digitar 5, atende o requisito inicial.
    cadastrarTerritorios(mapa, numTerritorios);

    // --- Alocacao Dinamica da Missao (Requisito) ---
    // Aloca a memoria para a string da missao
    missaoJogador = (char*)malloc(TAM_MISSAO * sizeof(char));
    if (missaoJogador == NULL) {
        printf("Erro: Falha ao alocar memoria para a missao.\n");
        liberarMemoria(mapa, NULL);
        return 1;
    }

    // --- Sorteio da Missao (Requisito) ---
    atribuirMissao(&missaoJogador, missoesEstrategicas, NUM_MISSOES);
    exibirMissao(missaoJogador);

    // --- Simular Turnos de Ataque e Verificacao ---
    int idAtacante, idDefensor;
    
    printf("\n--- Fase de Ataque (Indices de 0 a %d) ---\n", numTerritorios - 1);
    
    while (1) {
        // 5. Exibicao Condicional: Verifica a missao ao final de cada turno (ou inicio, para quebrar o loop)
        if (verificarMissao(missaoJogador, mapa, numTerritorios)) {
            printf("\n🎉 **MISSAO CUMPRIDA!** O jogador VENCEU o jogo!\n");
            break;
        }

        printf("\nDigite o ID do territorio ATACANTE (ou -1 para sair): ");
        if (scanf("%d", &idAtacante) != 1 || idAtacante == -1) break;
        
        printf("Digite o ID do territorio DEFENSOR: ");
        if (scanf("%d", &idDefensor) != 1) continue;

        // Validacao de indices
        if (idAtacante < 0 || idAtacante >= numTerritorios || 
            idDefensor < 0 || idDefensor >= numTerritorios || 
            idAtacante == idDefensor) {
            printf("⚠️ Indices invalidos ou territorios iguais. Tente novamente.\n");
            continue;
        }

        // Requisito Tecnico: Validacao de Ataque (Nao atacar o proprio)
        if (strcmp(mapa[idAtacante].cor, mapa[idDefensor].cor) == 0) {
            printf("⚠️ Ataque Invalido! Voce nao pode atacar um territorio da sua propria cor (%s).\n", mapa[idAtacante].cor);
            continue;
        }

        printf("\n💥 BATALHA: %s (%s) vs %s (%s)\n", 
               mapa[idAtacante].nome, mapa[idAtacante].cor,
               mapa[idDefensor].nome, mapa[idDefensor].cor);

        // Funcao de Ataque (Uso de Ponteiros)
        atacar(&mapa[idAtacante], &mapa[idDefensor]);
        
        // Exibicao Pos-Ataque (Requisito)
        printf("\n--- Mapa Atualizado ---\n");
        exibirMapa(mapa, numTerritorios);
    }

    // --- Gerenciamento de Memoria ---
    liberarMemoria(mapa, missaoJogador);

    return 0;
}

// --- Implementacao das Funcoes Modulares ---

/**
 * @brief Permite o usuario preencher os dados dos territorios.
 */
void cadastrarTerritorios(Territorio* mapa, int tamanho) {
    int i;
    // Laco 'for' para preencher os dados dos territorios (Requisito)
    for (i = 0; i < tamanho; i++) {
        printf("--- Cadastro Territorio %d de %d ---\n", i + 1, tamanho);
        
        // Interface simples e clara (Usabilidade)
        printf("Nome (max 29): ");
        scanf(" %29[^\n]", mapa[i].nome); 
        
        printf("Cor (dono, max 9): ");
        scanf(" %9s", mapa[i].cor);
        
        printf("Tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 1) {
             mapa[i].tropas = 1; 
             printf("Tropas ajustadas para 1.\n");
        }
    }
    printf("\n✅ Cadastro concluido. Mapa inicial:\n");
    // Exibicao dos dados apos preenchimento (Requisito)
    exibirMapa(mapa, tamanho);
}

/**
 * @brief Exibe o estado atual do mapa com formatacao clara.
 */
void exibirMapa(const Territorio* mapa, int tamanho) {
    printf("| %-4s | %-15s | %-8s | %-6s |\n", "ID", "TERRITORIO", "COR", "TROPAS");
    printf("|------|-----------------|----------|--------|\n");
    for (int i = 0; i < tamanho; i++) {
        printf("| %-4d | %-15s | %-8s | %-6d |\n",
               i,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
    printf("---------------------------------------------\n");
}

/**
 * @brief Sorteia uma missao e a armazena dinamicamente.
 * @param missaoDestino Ponteiro para o ponteiro char* (passagem por referencia)
 */
void atribuirMissao(char** missaoDestino, const char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    
    // Copia a string sorteada para a memoria alocada (*missaoDestino)
    strcpy(*missaoDestino, missoes[indiceSorteado]);
    printf("> Missao sorteada e atribuida ao jogador.\n");
}

/**
 * @brief Exibe a missao atual do jogador (Interface intuitiva).
 * @param missao A string da missao (passagem por valor - endereco).
 */
void exibirMissao(const char* missao) {
    printf("\n===================================\n");
    printf("📜 Sua Missao Secreta: **%s**\n", missao);
    printf("===================================\n");
}

/**
 * @brief Avalia se a missao do jogador foi cumprida (logica simples).
 * @param missao A string da missao (passagem por referencia).
 * @param mapa O vetor de territorios (passagem por referencia).
 * @return 1 se a missao foi cumprida, 0 caso contrario.
 */
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho) {
    // --- Logica Simples de Verificacao (Exemplo) ---
    
    // Missao: Eliminar todas as tropas da cor VERMELHA.
    if (strstr(missao, "VERMELHA") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            // Se houver qualquer territorio VERMELHA com tropas, a missao falha
            if (strcmp(mapa[i].cor, "VERMELHA") == 0 && mapa[i].tropas > 0) {
                return 0; 
            }
        }
        return 1; // Nao sobrou VERMELHA com tropas
    }
    
    // Missao: Conquistar 3 territorios com mais de 5 tropas.
    if (strstr(missao, "mais de 5 tropas") != NULL) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas > 5) {
                count++;
            }
        }
        if (count >= 3) return 1;
    }
    
    return 0; // Missao nao cumprida
}


/**
 * @brief Simula um ataque entre dois territorios e atualiza seus dados (Uso de Ponteiros).
 * @param atacante Ponteiro para a struct do territorio atacante.
 * @param defensor Ponteiro para a struct do territorio defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // Validacao: O atacante deve ter pelo menos 2 tropas para atacar
    if (atacante->tropas <= 1) { 
        printf("⚠️ %s nao tem tropas suficientes (minimo 2) para atacar. Ataque cancelado.\n", atacante->nome);
        return;
    }
    
    // Requisito: Simula rolagem de dados (1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("   - Dados lancados: ATACANTE (%d) vs DEFENSOR (%d)\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // --- Atacante Venceu! (Atualizacao de Dados) ---
        printf("   - Resultado: **%s VENCE**! %s foi conquistado.\n", atacante->nome, defensor->nome);
        
        // Requisito: Transfere metade das tropas
        int tropasTransferidas = defensor->tropas / 2;
        
        defensor->tropas -= tropasTransferidas; 
        atacante->tropas += tropasTransferidas;
        
        // Requisito: Mudar a cor do defensor (dono)
        strcpy(defensor->cor, atacante->cor);
        
        printf("   - %s assume o controle e %d tropas foram transferidas.\n", 
               defensor->nome, tropasTransferidas);

    } else {
        // --- Atacante Perdeu! (Atualizacao de Dados) ---
        printf("   - Resultado: **%s DEFENDEU** o ataque!\n", defensor->nome);
        
        // Requisito: O atacante perde uma tropa.
        atacante->tropas--;
        
        printf("   - %s perdeu 1 tropa. Tropas restantes: %d.\n", atacante->nome, atacante->tropas);
    }
}

/**
 * @brief Libera toda a memoria alocada dinamicamente (mapa e missao).
 * @param mapa Ponteiro para o inicio do vetor de territorios alocado.
 * @param missao Ponteiro para a string de missao alocada.
 */
void liberarMemoria(Territorio* mapa, char* missao) {
    printf("\n--- Liberando Memoria ---\n");
    // Requisito: Uso de free() para evitar vazamentos de memoria
    if (mapa != NULL) {
        free(mapa);
        printf("> Memoria do Mapa (Territorios) liberada.\n");
    }
    if (missao != NULL) {
        free(missao);
        printf("> Memoria da Missao liberada.\n");
    }
    printf("> Fim do programa.\n");
}
