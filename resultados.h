/*
Ficheiro resultados.h
Header que serve de aopio ao ficheio resultados.c e que tornará possivel que outros ficheiros acedam às funções que guardam a pontuação do jogador
no ficheiro "resultados .txt"
*/

//----------------------------------------------------------------------INCLUDES----------------------------------------------------------------------
#include "header_base.h" //funções básicas

//----------------------------------------------------------------------MACROS----------------------------------------------------------------------
#define MAX_DIM_MATRIZ_RESULTADOS 100

//----------------------------------------------------------------------PROTÓTIPOS---------------------------------------------------------------------

void fimDoJogo(int pontos);
void bubleSortExtracaoFichResultados(int linha, int pontos[MAX_DIM_MATRIZ_RESULTADOS], char nome[MAX_DIM_MATRIZ_RESULTADOS][23]);
int guardaNovamenteNoficheiroResultados(int linha, int pontosFichResultados[MAX_DIM_MATRIZ_RESULTADOS], 
	char nomeDoJogadorFichResultados[MAX_DIM_MATRIZ_RESULTADOS][23]);

