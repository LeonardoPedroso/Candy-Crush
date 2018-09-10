/*
Ficheiro graphics.h
Header que serve de aopio ao ficheio graphics.c e que tornará possivel que outros ficheiros acedam às funções que permitem desenhar as boolhas
e a interface gráfica bo ecrã  
*/

//


//----------------------------------------------------------------------INCLUDES----------------------------------------------------------------------
//incluir funções da biblioteca gráfica 
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
//e outras que permitem fazer operações entre listas e que já inclui o header base
#include "op_listas.h"

//----------------------------------------------------------------------MACROS----------------------------------------------------------------------
#define PI 3.141592654
#define DELAY 400 //em modo que não seja o de debug há um pequeno intervalo de espera para que se perceba o que está a aacontecer

//----------------------------------------------------------------------PROTÓTIPOS----------------------------------------------------------------------
//funções de controlo
void iniciarSDL2(DADOS dados);
void reset(DADOS dados, APBLOCO * OApontador);
void sairSDL2();

//funções de desennho na janela
void atualizaJanela(DADOS dados, APBLOCO * OApontador);
void desenha_butoes(DADOS dados);
void corBolha(int codigoCor, int cor[]);
void pinta_mais(int mais_x, int mais_y, DADOS dados, APBLOCO * OApontador);
void pinta_X(int mais_x, int mais_y, DADOS dados, APBLOCO * OApontador);

//funçoes de posição na janela e deteção de cliques
int deteta_clique(DADOS dados, APBLOCO * OApontador);
int CoordenadasMatriztransformaParaRefJanela(DADOS dados, int coordenada,  int eixo);
float distancia(int x1, int y1 , int x2, int y2);




