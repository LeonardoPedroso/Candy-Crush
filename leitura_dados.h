/*
Ficheiro leitura_dados.h
Header que serve de aopio ao ficheio leitura_dados.c e que tornará possivel que outros ficheiros acedam às funções que permitem interpretar 
as indicações dadas pelo jogador no terminal.
*/

//----------------------------------------------------------------------INCLUDES----------------------------------------------------------------------
#include "header_base.h"
//gerar números "aleatórios"
#include <time.h>

//----------------------------------------------------------------------PROTÓTIPOS----------------------------------------------------------------------

int lerConfigDados(DADOS * dados, int argc, char const * argv[]);
void mensagemErroDados();
