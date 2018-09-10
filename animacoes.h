/*
Ficheiro animacoes.h
Header que serve de aopio ao ficheio animacoes.c e que tornará possivel que outros ficheiros acedam às funções permitem criar as animações de 
descida e eliminação de bolhas
*/

//----------------------------------------------------------------------INCLUDES----------------------------------------------------------------------
#include "header_base.h" //inclui as funções, macros básicos e variáveis estruturadas base
#include "graphics.h" //inclui as funções e macros que permitem desenhar na janela gráfica
#include "op_listas.h" //inclui as funções, macros e variáveis estruturadas que operam sobre as listas de bolhas

//--------------------------------------------------------------------VARIÁVEIS GLOBAIS----------------------------------------------------------------
extern SDL_Renderer* g_pRenderer; //renderer

//----------------------------------------------------------------------MACROS----------------------------------------------------------------------
#define ITERACOES_IMPLOSAO 10
#define DELAY_IMPLOSAO 9 //intervalo de tempo entre iterações da implossão (em ms)

#define DS 0.1 //distância em termos de coordenadas da matriz que uma bolha percore por iteração de descida
#define DELAY_DESCER 20 //intervalo de tempo entre iterações de descida das bolhas


//----------------------------------------------------------------------PROTÓTIPOS---------------------------------------------------------------------

//eliminação de bolhas
void atualizaJanela_animacoes_implosao(DADOS dados, APBLOCO * OApontador);
int elimina_bolhas_marcadas_animacao(DADOS dados, APBLOCO * OApontador);

//queda animada de bolhas
void adiciona_bolhas_animacao(APBLOCO * OApontador, DADOS dados);
void atualizaJanela_animacoes_descer_velhas(DADOS dados, APBLOCO * OApontador);
void atualizaJanela_animacoes_descer_novas(DADOS dados, APBLOCO * OApontador);
int CoordenadasMatriztransformaParaRefJanela_coordenadas_continuas(DADOS dados, float coordenada,  int eixo);

//troca de bolhas
int deteta_clique_troca_fixe(DADOS dados, APBLOCO * OApontador, TROCA_FIXE_INFO * troca_info);
void define_direcao(TROCA_FIXE_INFO * troca_info);
void mexe_bolhas_troca(DADOS dados, APBLOCO * OApontador, TROCA_FIXE_INFO troca_info);
void restringe_ds(DADOS dados, int * ds);
void atualizaJanela_animacao_troca(DADOS dados, APBLOCO * OApontador, TROCA_FIXE_INFO troca_info);
void normaliza_bolhas_trocadas_animacao(DADOS dados, APBLOCO * OApontador);
void neutraliza_marcas_animacao_troca(DADOS dados, APBLOCO * OApontador);
