#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>

#define tamanho_min_tabuleiro 2
#define tamanho_max_tabuleiro 10
#define min_jogadores 2
#define max_jogadores 10
#define atraso_impressao_us 30000
#define titulo "*** J O G O   D A   V E L H A ***"
#define linha "*********************************"

void imprime_com_animacao(char *str);
void cabeçalho();
void define_tamanho_tabuleiro(int *N);
void popular_tabuleiro(int N, char tabuleiro[N][N]);
void define_modalidade_do_jogo(int *modalidade);
void define_num_jogadores(int *n_jogadores);
void nomear_jogadores(int n_jogadores, char jogadores[n_jogadores][30]);
void pre_jogo(int n_jogadores, char jogadores[n_jogadores][30]);
void imprime_tabuleiro(int N, char tabuleiro[N][N]);
int coordenada_valida(int coordenada, int N);
int posicao_livre(int N, char tabuleiro[N][N], int coordenadas[2]);
int gravidade(int N, char tabuleiro[N][N], int coordenadas[2]);
int confere_posicao(int N, char tabuleiro[N][N], int coordenadas[2], int modalidade);
void escolha_coordenada(int N, char tabuleiro[N][N], int n_jogadores, char jogadores[n_jogadores][30], int coordenadas[2], int turno, char *marcadores);
int efetiva_jogada(int N, char tabuleiro[N][N], int coordenadas[2], int *turno, char *marcadores, int modalidade);
int venceu_vertical(int N, char tabuleiro[N][N], char marcador);
int venceu_horizontal(int N, char tabuleiro[N][N], char marcador);
int venceu_diagonal(int N, char tabuleiro[N][N], char marcador);
int venceu(int N, char tabuleiro[N][N], char marcador);
void passa_turno(int *turno, int n_jogadores);
void parabens_vencedor(char *jogador);
void fim_sem_vencedor();

int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.UTF8");

    // Define variáveis
    int N, n_jogadores, turno = 0, rodada = 0, coordenadas[2] = {-1, -2}, ganhador = -1, modalidade;
    char marcadores[10] = "XOZYKURAPE";

    // Definição do tamanho do tabuleiro
    define_tamanho_tabuleiro(&N);

    // Declara o tabuleiro
    char tabuleiro[N][N];

    // Prepara tabuleiro para jogo
    popular_tabuleiro(N, tabuleiro);

    // Define modalidade do jogo
    define_modalidade_do_jogo(&modalidade);

    // Define o número de jogadores
    define_num_jogadores(&n_jogadores);

    // Declara os jogadores
    char jogadores[n_jogadores][30];

    // Declara nome dos jogadores
    nomear_jogadores(n_jogadores, jogadores);

    // Tela de saudação aos jogadores
    pre_jogo(n_jogadores, jogadores);

    /* ===== JOGO INICIA ===== */
    while (rodada < N*N && ganhador < 0) {
        // Inicia evento de seleção de coordenadas
        escolha_coordenada(N, tabuleiro, n_jogadores, jogadores, coordenadas, turno, marcadores);

        // Tenta efetivar jogada, caso posição válida, marca posição e continua jogo
        if (efetiva_jogada(N, tabuleiro, coordenadas, &turno, marcadores, modalidade)) {
            // Conta uma rodada
            rodada++;

            // Verifica se o jogador atual venceu, se sim, finaliza o jogo
            if (venceu(N, tabuleiro, marcadores[turno])) {
                ganhador = turno;
                break;
            }

            // Realiza passagem ou reiniciação de turno
            passa_turno(&turno, n_jogadores);
        }
    }

    /* ===== JOGO FINALIZA ===== */
    if (ganhador >= 0) {
        parabens_vencedor(jogadores[ganhador]);
    } else {
        fim_sem_vencedor();
    }
}

void imprime_com_animacao(char* str) {
    /* 
        Imprime um caractere por vez
    */

    for (int i = 0; i < strlen(str); i++) {
        printf("%c", str[i]);
        usleep(atraso_impressao_us);
    }
}

void cabeçalho() {
    /* 
        Imprime o cabeçalho
    */
    printf("%s", titulo);
    printf("\n");
    printf("%s", linha);
    printf("\n\n");
}

void define_tamanho_tabuleiro(int *N) {
    /* 
        Solicita ao usuário o tamanho do tabuleiro
    */
   
    // Imprime cabeçalho com animação
    system("cls");
    imprime_com_animacao(titulo);
    printf("\n");
    imprime_com_animacao(linha);
    printf("\n\n");

    // Solicita tamanho do tabuleiro
    imprime_com_animacao("Digite o tamanho do tabuleiro (2-10): ");
    fflush(stdout);
    scanf("%d", N);

    // Validação
    while (*N < tamanho_min_tabuleiro || *N > tamanho_max_tabuleiro) {
        // Limpa o buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 

        // Ressolicita tamanho do tabuleiro
        system("cls");
        cabeçalho();
        imprime_com_animacao("Valor inválido. Digite um valor entre 2 e 10: ");
        fflush(stdout);
        scanf("%d", N);
    }

    // Limpa o buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);    
}

void popular_tabuleiro(int N, char tabuleiro[N][N]) {
    /* 
        Popula o tabuleiro de dimensão NxN com espaços vazios
    */

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tabuleiro[i][j] = ' ';
        }
    }
}

void define_modalidade_do_jogo(int *modalidade) {
    // Imprime cabeçalho
    system("cls");
    cabeçalho();

    // Solicita modalidade de jogo
    imprime_com_animacao("Escolha a modalidade de jogo: ");
    printf("\n");
    imprime_com_animacao("0 - Clássica");
    printf("\n");
    imprime_com_animacao("1 - Com Gravidade");
    printf("\n");
    fflush(stdout);
    scanf("%d", modalidade);

    // Validação
    while(*modalidade < 0 || *modalidade > 1) {
        // Imprime cabeçalho
        system("cls");
        cabeçalho();

        // Solicita modalidade de jogo novamente
        imprime_com_animacao("Opção inválida. Tente novamente: ");
        printf("\n");
        imprime_com_animacao("0 - Clássica");
        printf("\n");
        imprime_com_animacao("1 - Com Gravidade");
        printf("\n");
        fflush(stdout);
        scanf("%d", modalidade);
    }
}

void define_num_jogadores(int *n_jogadores) {
    /* 
        Solicita ao usuário o número de jogadores
    */

    // Imprime cabeçalho
    system("cls");
    cabeçalho();

    // Solicita número de jogadores
    imprime_com_animacao("Digite o número de jogadores (2-10): ");
    fflush(stdout);
    scanf("%d", n_jogadores);

    // Validação
    while (*n_jogadores < 2 || *n_jogadores > 10) {
        // Limpa o buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        system("cls");
        cabeçalho();
        imprime_com_animacao("Valor inválido. O número de jogadores deve ser entre 2 e 10: ");
        fflush(stdout);
        scanf("%d", n_jogadores);
    }

    // Limpa o buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void nomear_jogadores(int n_jogadores, char jogadores[n_jogadores][30]) {
    /* 
        Solicita ao usuário o nome dos jogadores.
    */

    // Itera sobre cada jogador
    for (int i = 0; i < n_jogadores; i++) {
        // Prepara texto de solicitação de nome
        char texto[30];
        sprintf(texto, "Digite o nome do Jogador %d: ", i + 1);

        // Imprime cabeçalho
        system("cls");
        cabeçalho();

        // Solicita e armazena nome do jogador i
        imprime_com_animacao(texto);
        fflush(stdout);
        fgets(jogadores[i], 30, stdin);
        jogadores[i][strcspn(jogadores[i], "\n")] = '\0';
    }
}

void pre_jogo(int n_jogadores, char jogadores[n_jogadores][30]) {
    /* 
        Imprime tela de saudação pré-jogo.
    */

    // Imprime cabeçalho
    system("cls");
    cabeçalho();

    // Apresente nome dos jogadores
    for (int i = 0; i < n_jogadores; i++) {
        char texto[51];
        sprintf(texto, "Nome do jogador %d: %s", i + 1, jogadores[i]);
        imprime_com_animacao(texto);
        printf("\n");
    }

    // Dá boa sorte aos jogadores
    imprime_com_animacao("Boa sorte jogadores ");
    for (int i = 0; i < n_jogadores; i++) {
        char texto[32];
        if (i < n_jogadores - 2) {
            sprintf(texto, "%s, ", jogadores[i]);
        } else if (i == n_jogadores - 2) {
            sprintf(texto, "%s e ", jogadores[i]);
        } else {
            sprintf(texto, "%s.", jogadores[i]);
        }
        
        imprime_com_animacao(texto);
    }
    printf("\n\n");

    // Aguarda 'Enter' para iniciar o jogo
    imprime_com_animacao("Aperte Enter para iniciar.");
    fflush(stdout);
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

void imprime_tabuleiro(int N, char tabuleiro[N][N]) {
    /* 
        Imprime tabuleiro
    */

    // Imprime índice da coluna
    printf("   ");
    for (int i = 0; i < N; i++) {
        printf(" %-3d", i); 
    }
    printf("\n");

    // Imprime as linhas, com índices e valores
    for (int i = 0; i < N; i++) {
        printf("%-2d ", i);
        for (int j = 0; j < N; j++) {
            printf(" %c ", tabuleiro[i][j]);
            if (j < N - 1) {
                printf("|");
            }
        }
        printf("\n");
        if (i < N - 1) {
            printf("   ");
            for (int j = 0; j < N; j++) {
                printf("----");
            }
            printf("\n");
        }
    }
    printf("\n\n");
}

int coordenada_valida(int coordenada, int N) {
    /* 
        Verifica se uma única coordenada é válida, através
        da condição de estar contido em N.
    */

    // Verifica se está contido em N
    if (coordenada >= 0 && coordenada <= N-1) {
        return 1;
    } else {
        return 0;
    }
}

int posicao_livre(int N, char tabuleiro[N][N], int coordenadas[2]) {
    /* 
        Verifica se a posição escolhida está livre, ou seja, não marcada.
    */

    // Verifica se a posição já não foi marcada
    if (tabuleiro[coordenadas[0]][coordenadas[1]] != ' ') {
        return 0;
    } else {
        return 1;
    }
}

int gravidade(int N, char tabuleiro[N][N], int coordenadas[2]) {
    /* 
        Verifica se a posição escolhida obedece a regra da gravidade.
    */

    // Verifica se obedece a gravidade
    if (coordenadas[0] + 1 != N && tabuleiro[coordenadas[0] + 1][coordenadas[1]] == ' ') {
        return 0;
    } else {
        return 1;
    }
}

int confere_posicao(int N, char tabuleiro[N][N], int coordenadas[2], int modalidade) {
    /* 
        Determina validez da posição, considerando hierarquia de critérios. Se não for válida,
        irá retornar o valor que indica o critério de maior hierarquia que fora desobedecido.
        Se válida, retorna 1.
    */

    if (modalidade == 0) { // Modalidade clássica
        // Confere a posição e retorna erro, de acordo com hierarquia
        if (!posicao_livre(N, tabuleiro, coordenadas)) {
            return 2;
        } else {
            return 1;
        }
    } else { // Modalidade com gravidade
        // Confere a posição e retorna erro, de acordo com hierarquia
        if (!posicao_livre(N, tabuleiro, coordenadas)) {
            return 2;
        } else if (!gravidade(N, tabuleiro, coordenadas)) {
            return 3;
        } else {
            return 1;
        }
    }    
}

void escolha_coordenada(int N, char tabuleiro[N][N], int n_jogadores, char jogadores[n_jogadores][30], int coordenadas[2], int turno, char *marcadores) {
    /* 
        Inicializa evento de escolha de coordenada pelo jogador, armazenando as suas escolhas de coordenadas.
    */

    // Itera sobre cada coordenada, Y e X
    for (int i = 0; i < 2; i++) {
        // Define o eixo a partir da iteração
        const char *eixo = (i == 0) ? "vertical" : "horizontal";

        // Imprime cabeçalho e tabuleiro
        system("cls");
        cabeçalho();
        imprime_tabuleiro(N, tabuleiro);

        // Solicita coordenadas ao jogador
        printf("%s (%c), escolha o valor para a coordenada %s: ", jogadores[turno], marcadores[turno], eixo);
        scanf("%d", &coordenadas[i]);

        // Valida coordenada. Se não válido, solicita novamente as coordenadas
        while (!coordenada_valida(coordenadas[i], N)) {
            system("cls");
            cabeçalho();
            imprime_tabuleiro(N, tabuleiro);
            printf("Valor inválido. Digite a coordenada %s entre 0 e %d: ", eixo, N-1);
            scanf("%d", &coordenadas[i]);
        }
    }
}

int efetiva_jogada(int N, char tabuleiro[N][N], int coordenadas[2], int *turno, char *marcadores, int modalidade) {
    /* 
        Tenta efetivar a jogada com auxílio da função 'confere_posicao'.
        Dará tratamento para cada erro retornado pela função 'confere_posicao'.
        Se nenhum erro, marca a posição escolhida pelo jogador e troca de turno.
    */

    int efetivou = 0;

    // Determina evento, considerando valor retornado pela função 'confere_posicao'
    switch (confere_posicao(N, tabuleiro, coordenadas, modalidade)) {
        // Se posição já marcada
        case 2:
            for (int i = 0; i <= 3; i++) {
                // Informa erro e reinicia turno
                system("cls");
                printf("A posição escolhida já foi marcada. Por favor, escolha uma outra posição. (%ds)", 3-i);
                sleep(1);
            }
            break;

        // Se posição não obedece gravidade
        case 3:
            // Informa erro e reinicia turno
            for (int i = 0; i <= 3; i++) {
                system("cls");
                printf("A posição escolhida não obedece a gravidade. Por favor, escolha uma outra posição. (%ds)", 3-i);
                sleep(1);
            }
            break;

        // Se posição válida
        default: 
            // Marca posição
            tabuleiro[coordenadas[0]][coordenadas[1]] = marcadores[*turno];

            // Reseta variáveis de coordenadas
            coordenadas[0] = -1;
            coordenadas[1] = -2;

            // Definie que a jogada foi efetivada
            efetivou = 1;
            break;
    }

    // Retorna status de efetivação
    return efetivou;
}

int venceu_vertical(int N, char tabuleiro[N][N], char marcador) {
    for (int i = 0; i < N; i++) {
        // Define contagem de sequência
        int sequencia = 0;

        // Itera por linha
        for (int j = 0; j < N; j++) {
            // Verifica condição de sequência
            if (tabuleiro[j][i] != marcador) {
                break;
            } else {
                sequencia++;
            }
        }

        // Se existe sequência é igual N, retorna verdadeiro (venceu)
        if (sequencia == N) {
            return 1;
        }
    }

    // Por padrão, retorna falso (não venceu)
    return 0;
}

int venceu_horizontal(int N, char tabuleiro[N][N], char marcador) {
    for (int i = 0; i < N; i++) {
        // Define contagem de sequência
        int sequencia = 0;

        // Itera por coluna
        for (int j = 0; j < N; j++) {
            // Verifica condição de sequência
            if (tabuleiro[i][j] != marcador) {
                break;
            } else {
                sequencia++;
            }
        }

        // Se existe sequência é igual N, retorna verdadeiro (venceu)
        if (sequencia == N) {
            return 1;
        }
    }

    // Por padrão, retorna falso (não venceu)
    return 0;
}

int venceu_diagonal(int N, char tabuleiro[N][N], char marcador) {
    
    // Itera sobre a diagnoais identidade e inversa
    for (int i = 0; i < N; i++) {
        // Define contagem de sequência
        int sequencia_diag_identidade = 0;
        int sequencia_diag_inversa = 0;

        // Verifica posições identidade e inversas
        if (tabuleiro[i][i] != marcador && tabuleiro[i][N-i] != marcador) {
            break;
        } else if (tabuleiro[i][i] == marcador && tabuleiro[i][N-i] != marcador) {
            sequencia_diag_identidade++;
        } else if (tabuleiro[i][i] != marcador && tabuleiro[i][N-i] == marcador) {
            sequencia_diag_inversa++;
        } else {
            sequencia_diag_identidade++;
            sequencia_diag_inversa++;
        }

        // Se existe sequência é igual N, retorna verdadeiro (venceu)
        if (sequencia_diag_identidade == N || sequencia_diag_inversa == N) {
            return 1;
        }
    }

    // Por padrão, retorna falso (não venceu)
    return 0;
}

int venceu(int N, char tabuleiro[N][N], char marcador) {
    if (venceu_vertical(N, tabuleiro, marcador) || venceu_horizontal(N, tabuleiro, marcador) || venceu_diagonal(N, tabuleiro, marcador)) {
        return 1;
    } else {
        return 0;
    }
}

void passa_turno(int *turno, int n_jogadores) {
    // Troca de turno
    if (*turno < n_jogadores - 1) {
        (*turno)++;
    } else {
        *turno = 0;
    }
}

void parabens_vencedor(char *jogador) {
    system("cls");
    cabeçalho();
    char texto[80];
    sprintf(texto, "Parabéns, %s! Você foi o vencedor do jogo.", jogador);
    imprime_com_animacao(texto);
}

void fim_sem_vencedor() {
    system("cls");
    cabeçalho();
    imprime_com_animacao("Ops... Parece que ninguém foi vencedor dessa vez.");
}