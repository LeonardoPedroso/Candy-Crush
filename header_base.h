/*
Ficheiro header_base.h
Header que inclui as bibliotecas básicas, estruturas básicas e MACROS que são usados por grande parte dos ficheiros que compoem o jogo
*/

//prevenir inclusõs múltiplas
#ifndef INCLUIDO
#define INCLUIDO

//----------------------------------------------------------------------INCLUDES----------------------------------------------------------------------
#include <stdlib.h> //necessário para alocação de memória
#include <math.h> 
#include <stdio.h> //funções básicas de C
#include <string.h> //operãos de strings

//------------------------------------------------------------------ VARIAVEIS ESTRUTURADAS -------------------------------------------------------------------
typedef struct estruturaDeDados
{
	int larguraJanela; //em bolhas
	int alturaJanela; //em bolhas
	int dimBolha;
	int debug;
	int fixe;

	int windowSizeX;
	int windowSizeY;
	float dl;

	int altura_barra_superior;
	int pontos;

}DADOS;

//estrutura de dados que guarda informação relevante para a animação da bolha
typedef struct troca
{
	int x0; //posição do rato quando fez o primeiro clique
	int y0;

	int i; //coordenadas da bolha clicada
	int j;

	int direcao; //direcao da troca (1-Horizontal 2-Vertical)
	int aTrocar; //o utilizador ainda está a premir o rato pelo que a animação continua
	
}TROCA_FIXE_INFO;

//----------------------------------------------------------------------MACROS----------------------------------------------------------------------
//lógica booleana
#define TRUE 1
#define FALSE 0

//limites janela
#define LARGURA_MINIMA1 (30*12+30*(0.1)*(12+1))
#define LARGURA_MINIMA  (30*5+30*(0.1)*(5+1))
#define LARGURA_MAXIMA (20*60+20*(0.1)*(60+1))
#define ALTURA_MAXIMA (1.5*20 + 20*30 + (0.1)*20*(30+1) + 1.75*20)

//constantes definidas pelos programadores
#define DL 0.05

//macros funcao 'CoordenadasMatriztransformaParaRefJanela'
#define EIXO_X 0
#define EIXO_Y 1

//debug
#define LINHA {printf("-----%d--------\n", __LINE__);}

#endif
