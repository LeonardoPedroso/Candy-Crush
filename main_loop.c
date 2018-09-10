/*
Programação - Projeto Final

Alunos:
Iara Figueiras - 89668
Leonardo Duarte - 89691

Jogo com o nível máximo implementado (nível 3) e que incorpora todos o requesitos especificados no enunciado
Para uma melhor experiência adicionar a flag -f ao correr o programa e entrar no modo fixe! (é uma surpresa:))
r
Este jogo foi desenvolvido segundo a norma C99
gcc -c -D_THREAD_SAFE -I/usr/local/include/SDL2 -Wall -pedantic -std=c99  *.c && gcc -D_THREAD_SAFE -I/usr/local/include/SDL2 -L/usr/local/lib -D_REENTRANT  -lm -lSDL2 -lSDL2_gfx  *.o -o run_bubbles && ./run_bubbles
*/

/*
Ficheiro main_loop.c
Este ficheiro é aquele em que são evidentes todas as operações efetuadas no decorrer do jogo, incluindo ,portanto, a função
main que por sua vez faz uso da função "loopDoJogo" auxilidada por "loop_verificacao_caminhos" que trocam, procuram caminhos e eliminam
bolhas e guardam o resultado da pontuação no ficheiro "resultados.txt";
Assim, são incluidos diversos ficheiros que permitem aceder a funções que tornam possivel a execução destas tarefas.
*/

//------------------------------------------------------------------INCLUDES-------------------------------------------------------------------
#include "leitura_dados.h" //ficheiro com funções que permitem aceder e intrepretar os dados introduzidos no terminal
#include "op_listas.h" //ficheiro com funções necessárias para realizar operações com as bolhas nas listas (e alguns extras :))
#include "graphics.h" //ficheiro com todas as funções necessárias para a representação gráfica do jogo
#include "animacoes.h" //ficheiro que dá acesso a funções da representaçãodas animações do jogo
#include "resultados.h" //ficheiro que dá acesso a funções que gurdarão e ordenarão a pontuação do jogador no ficheiro de resultados

//----------------------------------------------------------------------MAIN----------------------------------------------------------------------
int main(int argc, char const *argv[])
{	

	DADOS dados; //declararção da variável que será utilizada para guardar os dados gerais do programa

	//invoca a função lerConfigDados para extrair os dados do terminal, se esta função restornar 1 houve um erro é mostrada a mensagem de erro
	if(lerConfigDados(&dados, argc, argv) == 1) 
  		mensagemErroDados();
  	
  	//iniciaçlizar o apontador que servirá de acesso a todas as bolhas no programa 
  	APBLOCO * OApontador = NULL;

  	iniciarSDL2(dados);//inicia a biblioteca gráfica e abre a janela do jogo
  
	loopDoJogo(&dados, OApontador); //executa o loop do jogo que só terminará quando o utilizador assim o definir ou não houver opções de jogada

  	sairSDL2(); //destruir o renderer, a janela e fecha a execução da biblioteca gráfca
  	
  	fimDoJogo(dados.pontos); //guarda a pontuação do jogador ordenadamente no ficheiro de resultados

	return 0; 
}

//------------------------------------------------------------------------LOOP-------------------------------------------------------------------//

/*
Função principal do programa responsável pelo ciclo do jogo
Recebe o vetor dados para aceder as definições de jogabilidade e da janela, por endereço para alguns valores (pontuação) estarem disponiveis
após esta função ter terminado.
Recebe também o apontador (inicializado a NULL) que dará acesso às posições e respetivas cores das bolhas para que no deccorer desta função 
se possa monitorizar as alterações e consequencias nestes valores das ações do jogador.
É responsável pelo seguinte ciclo de ações:
-> detetar o clique na janela
-> ordenar a troca, se for caso disso e correr a função que procura e elimina caminhos de bolhas
-> verificar se o jogo continua, acaba, o se é recomeçado
*/
void loopDoJogo(DADOS * dados, APBLOCO * OApontador)
{
	int jogoContinua = TRUE; //variável que indica se o jogo reúne todas as condições para continuar
	int jogoRepete = FALSE; //variavel que indica se o jogo se repetirá (ao carregar no botao novo jogo)
	int bolhas_selecionadas = 0; //variável que guardará o número de bolhas selecionadas para troca
	TROCA_FIXE_INFO troca_info;
	SDL_Event event; //evento para detetar cliques
	do //inicio do ciclo de um novo jogo	
	{	
		OApontador = criaListas(*dados); //criar as listas de bolhas e atribuir o endereço de acesso à variável correspondente
		if (dados->fixe == TRUE) //no modo fixe as bolhas iniciais não podem formar já caminhos inicialmente
			remove_bolhas_iniciais_em_caminhos(*dados, OApontador);
		//inicializar variáveis de jogabilidade
  		dados->pontos = 0; 
		jogoRepete = FALSE; 
		jogoContinua = TRUE;
		bolhas_selecionadas = 0;
		//inicializar variáveis de troca
		troca_info.x0 = -1;
		troca_info.y0 = -1;
		troca_info.i = -1;
		troca_info.j = -1;
		troca_info.direcao = 0;
		reset(*dados, OApontador); //desenha a disposição inicial das bolhas na janela
		while (jogoContinua == TRUE) //inicio do loop de execução dos comandos do jogo
		{	SDL_WaitEvent(&event); //aguarda interação do utilizador
			switch (event.type)
			{	case SDL_QUIT: //caregar no X da Janela
				{ 	jogoContinua = FALSE;
					jogoRepete = FALSE;
					break;
				}
				case SDL_MOUSEBUTTONDOWN: //no modo fixe ao pressionar o rato é logo intrepretada a informação da troca para que se proceda à animação
				{	if (dados->fixe == TRUE)
					{	switch(event.button.button)
						{	case SDL_BUTTON_LEFT: //no caso de ser o botao esquerdo verifica se carregou numa bolha ou num botão
            				{	troca_info.aTrocar = deteta_clique_troca_fixe(*dados, OApontador, &troca_info); //intrepretar informação da troca
            					break;
            				}
						}		
					}
					break;
				}
				case SDL_MOUSEMOTION: //no modo fixe se o utilizador estiver a trocar uma bolha ao mexer no rato a posição da bolha a trocar é atualizada
				{	
					if (troca_info.aTrocar == TRUE)
					{	if (troca_info.direcao == 0) //se a direção for indefinida, define-a
							define_direcao(&troca_info);
						mexe_bolhas_troca(*dados, OApontador, troca_info); //calcula a posição da bolha e mostra a animação da troca
					}
					break;
				}
				case SDL_MOUSEBUTTONUP: //quando um botao do rato deixar de ser premido
				{	switch (event.button.button)
            		{	case SDL_BUTTON_LEFT: //no caso de ser o botao esquerdo verifica se carregou numa bolha ou num botão
            			{
            				if (dados->fixe == TRUE)
            					mouse_up_fixe(dados, OApontador, &jogoRepete, &jogoContinua, &troca_info);
            				else
            					mouse_up_usual(dados, OApontador, &jogoRepete, &jogoContinua, &bolhas_selecionadas);
            			}					       		
            		}
            		break;
            	}      	
			}
		}
		OApontador = byebye_listas(*dados, OApontador); //no final de cada jogo a memoria alocada é libertada e OApontador = NULL
	}while(jogoRepete == TRUE); //se carregar em novo jogo 'jogoRepete' vai tomar o valor TRUE e o jogo recomeca com as mesmas opcoes de inicialização
}

/*
Função cujo propósito é auxiliar a função loopDOJogo
Esta função recebe a estrutura de dados para que se possa aceder às configurações gerais do jogo (por endereço para que as alterações
efetuadas sejam também alteradas na função loopDoJogo e, consequentemente na main) e o apontador de acesso às listas que servem de 
suporte à organização das bolhas
Assim, é responsável por todas as ações que tenham de ser executadas qunado o botao do rato é deixado de ser premido, nomeadamete a seleção 
das bolhas para a troca e no modo fixe marca o fim da animação de troca
*/
void mouse_up_fixe(DADOS * dados, APBLOCO * OApontador, int * jogoRepete, int * jogoContinua, TROCA_FIXE_INFO * troca_info)
{
    switch(deteta_clique(*dados, OApontador)) //verirficar no odo fixe se clicou os butoes
	{	case 3: //botao fim do jogo
		{	
			if (troca_info->aTrocar == FALSE)
			{
				*jogoContinua = FALSE; 
				*jogoRepete =FALSE; 
				break;
			}
		} 
		case 4: //botao novo jogo
		{	
			if (troca_info->aTrocar == FALSE)
			{
				*jogoContinua = FALSE; 
				*jogoRepete = TRUE; 
				break;
			}
		}
		default: //se não clicou nos butões avalia se a troca com animação foi bem sucedida
		{	if(troca_info->aTrocar == TRUE)
			{	troca_info->aTrocar = FALSE;
				//troca as bolhas se os desvios fores grandes o suficiente ou retorna as bolhas às posições iniciais
				normaliza_bolhas_trocadas_animacao(*dados, OApontador); 
				loop_verificacao_caminhos(dados, OApontador); //verifica se a troca efetuou caminhos
			}	
		}
	}
 }

void mouse_up_usual(DADOS * dados, APBLOCO * OApontador, int * jogoRepete, int * jogoContinua, int * bolhas_selecionadas)
{   
    switch(deteta_clique(*dados, OApontador))
	{	case 3: //botao fim do jogo
		{	*jogoContinua = FALSE; 
			*jogoRepete =FALSE; 
			break;
		} 
		case 4: //botao novo jogo
		{	*jogoRepete = TRUE; 
			*jogoContinua = FALSE; 
			break; 
		}
		case 0: break; //não clicou em nada
		case -1: //se o utilizador carregou na bolha que estava já marcada a seleção dessa bolha é anulada
		{	*bolhas_selecionadas = 0; //o numero de bolhas selecinadas é anulado
			reset(*dados, OApontador); //apaga o mais na bolha selecionada
			break;
		}
		case 1: 
		{	reset(*dados, OApontador); //pinta o + na bolha selecionada
			*bolhas_selecionadas += 1;
			//se forem selecinadas duas bolhas corre o ciclo de verificação de caminhos até que a configuração das bolhas não forme caminhos
			if (*bolhas_selecionadas == 2) 
			{	*bolhas_selecionadas = 0;
				loop_verificacao_caminhos(dados, OApontador);
				if(verifica_estado_do_jogo(OApontador, *dados) == FALSE) //verifica se existem mais hipoteses de jogada
				{	DEBUG("sair. Não há mais hipoteses de jogada.");
					*jogoContinua = FALSE;
					*jogoRepete = FALSE;	
				}	
			}
			break;
		}
	}
}

/*
Função cujo propósito é auxiliar a função loopDoJogo
Esta função recebe a estrutura de dados para que se possa aceder às configurações gerais do jogo (por endereço para que as alterações
efetuadas sejam também alteradas na função loopDoJogo e, consequentemente na main) e o apontador de acesso às listas que servem de 
suporte à organização das bolhas
Assim, é responsável pelo ciclo de ações:
-> marcar as bolhas a ser eliminadas
-> eliminar nolhas marcadas
até que não haja mais bolhas a ser eliminadas
*/
void loop_verificacao_caminhos(DADOS * dados, APBLOCO * OApontador)
{

	if(dados->debug == TRUE && dados->fixe == FALSE) //se em modo de debug espera que o utilizador carregue em enter
		DEBUG("trocar!");
	else if(dados->fixe == FALSE) //caso contrário há um pequeno delay para que seja possivel acompanhar o deccorer das ações
		SDL_Delay(DELAY);

	if (dados->fixe == FALSE)
	{
		troca_bolhas(*dados, OApontador); //troca as bolhas selecionadas
		reset(*dados, OApontador); //desenha as bolhas no ecrã já trocadas
	}
	while(TRUE) //até ser dada a indicação de break (não se formam mais caminhos) corre as ações porque esta função é responsável
	{
		if(dados->debug == TRUE)
			DEBUG("procurar e marcar bolhas a ser eliminadas!");
		else if(dados->fixe == FALSE)
			SDL_Delay(DELAY);

		if(marcar_bolhas_a_eliminar(*dados, OApontador) == FALSE) //marcar as bolhas a eliminar
			break; //se não houver bolhas a eliminar a configuração das bolhas não forma mais caminhos e esta função termina
		
		if(dados->debug == TRUE)
			reset(*dados, OApontador); //no modo de debug as bolhas a ser eliminadas serão marcadas com um X

		if(dados->debug == TRUE) 
			DEBUG("eliminar bolhas marcadas!");
		else if(dados->fixe == FALSE)
			SDL_Delay(DELAY);
		
		if (dados->fixe == TRUE) //no modo fixe são usadas funções diferentes que complementam o jogo com animações
		{
			dados->pontos += elimina_bolhas_marcadas_animacao(*dados, OApontador);
			atualizaJanela_animacoes_descer_velhas(*dados,OApontador); //anima descida de bolhas acima de bolhas que foram eliminadas
		}
		else
		{	//as bolhas marcadas são eliminadas e os pontos aumentados no numero de bolhas 
			dados->pontos += elimina_bolhas_marcadas(*dados, OApontador); 
			reset(*dados, OApontador);
		}

   		if (dados->fixe == TRUE)//no modo fixe são adicionadas bolhas que substituem as removidas
   		{
   			if (dados->debug == TRUE)
   				DEBUG("adicionar bolhas, porque este é o modo fixe!");
   			//anima adição de bolhas
   			adiciona_bolhas(OApontador, *dados, TRUE); 
			atualizaJanela_animacoes_descer_novas(*dados, OApontador); 
   		}
	}
}






