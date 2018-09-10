/*
Ficheiro op_listas.h
Header que serve de aopio ao ficheio op_listas.c e que tornará possivel que outros ficheiros acedam às funções que permitem fazer operações
sobre as listas  
*/

//prevenir inclusões múltiplas
#ifndef INCLUDED 
#define INCLUDED

//----------------------------------------------------------------------INCLUDES----------------------------------------------------------------------
#include "header_base.h"

//----------------------------------------------------------------------MACROS----------------------------------------------------------------------

//Tipos de marcas possíveis para bolhas
#define MARCADA_PARA_TROCA 1
#define SOFREU_ALTERACAO 2
#define MARCADA_PARA_ELIMINACAO_TEMP 3
#define MARCADA_PARA_ELIMINACAO 4
#define ANIMACAO_DESCER 100
#define ANIMACAO_TROCA 300


//------------------------------------------------------------------ VARIAVEIS ESTRUTURADAS -------------------------------------------------------------------
//definição da estrutura que guarda a toda a informação necessária referente a uma bolha
typedef struct estrutura_bolha
{
	int cor;
	int marca;
}BOLHA;

/*definição da estrutura dos blocos da lista dinâmica que servirá de suporte à disposição das bolhas
è composta pelo espaço util reservado prar a a informação da bolha e um apontador para o próximo bloco*/
typedef struct bloco_lista
{
	BOLHA bolha;
	struct bloco_lista * prox;

}BLOCO;

//redefinição de apontador para a variàvel estruturada BLOCO como APBLOCO para melhor compreensão do código
typedef BLOCO * APBLOCO;


//----------------------------------------------------------------------PROTÓTIPOS----------------------------------------------------------------------
//protótipos das duas funções definidas em main_loop.c
void loopDoJogo(DADOS * dados, APBLOCO * OApontador);
void loop_verificacao_caminhos(DADOS * dados, APBLOCO * OApontador);
void mouse_up_fixe(DADOS * dados, APBLOCO * OApontador, int * jogoRepete, int * jogoContinua, TROCA_FIXE_INFO * troca_info);
void mouse_up_usual(DADOS * dados, APBLOCO * OApontador, int * jogoRepete, int * jogoContinua, int * bolhas_selecionadas);

//------------------------------------------------protótipos das funções que operaram sobre as listas -------------
APBLOCO* criaListas(DADOS dados);


//posição
APBLOCO apbloco_bolha_em_i_j(APBLOCO * OApontador, int i, int j, DADOS dados);

//troca de bolhas
void troca_bolhas(DADOS dados, APBLOCO * OApontador);
void troca_em_coluna(APBLOCO * OApontador, APBLOCO anterior, APBLOCO bolhaTroca_1, int coluna);
void troca_entre_colunas(APBLOCO * OApontador, DADOS dados, APBLOCO bolhaTroca_1,  int i, int j);
void eliminar_marcas_troca(APBLOCO * OApontador, DADOS dados, int j);

//eliminação de bolhas
int marcar_bolhas_a_eliminar(DADOS dados, APBLOCO * OApontador);
int marcar_bolhas_a_eliminar_vizinhanca(DADOS dados, APBLOCO * OApontador, int i, int j);
void marca_bolhas_candidatas_a_0(DADOS dados, APBLOCO * OApontador);
void marca_bolhas_candidatas_eliminacao_permanente(DADOS dados, APBLOCO * OApontador);
int elimina_bolhas_marcadas(DADOS dados, APBLOCO * OApontador);
void remove_bolhas_iniciais_em_caminhos(DADOS dados, APBLOCO * OApontador);

//adição de bolhas
void adiciona_bolhas(APBLOCO * OApontador, DADOS dados, int com_animacao);

//controlo
int verifica_estado_do_jogo(APBLOCO * OApontador, DADOS dados);
int avalia_matriz(int numero_de_cores[9][2]);
APBLOCO * byebye_listas(DADOS dados, APBLOCO * OApontador);


//------------------------------------------------protótipos das funções de debug sobre operções de listas ----------
//debug
void estado(DADOS dados, APBLOCO * OApontador);
void DEBUG(char str[]);

#endif
