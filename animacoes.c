/*
Ficheiro animacoes.c
Este é o ficheiro onde estão definidas todas as funções que contribuem para a a animacao de algumas componetes do jogo, nomeadamente
a descida e eliminação de bolhas
*/

//----------------------------------------------------------------------INCLUDES----------------------------------------------------------------------
#include "animacoes.h" //protótipos, funções extra, macros 

//----------------------------------------------------------------------FUNÇÕES----------------------------------------------------------------------

//------------------------------------------------ eliminação de bolhas --------------------------------------------
/*
Função que elimina todas as bolhas que estejam marcadas para eliminação permante e anima essa eliminação com uma implosão
Para além disso marca todas as bolhas acima das bolhas eliminadas com informação referente às posições relaticvas dessas bolhas em casa coluna
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
Retorna: o numero de bolhas eliminadas e que será usado na contagem dos pontos do jogador
*/
int elimina_bolhas_marcadas_animacao(DADOS dados, APBLOCO * OApontador)
{
	int bolhas_rebentadas_total = 0; //variável que guarda o número de bolhas que foram rebentadas
  	atualizaJanela_animacoes_implosao(dados, OApontador); //anima a implosão	

  	for (int j = 0; j < dados.larguraJanela; ++j)//percorre todas as colunas
  	{
   		APBLOCO step = NULL, anterior = NULL; //variáveis para varrer todas as bolhas
   		APBLOCO apaga; //variável que permite desalocar memória sem perder o ponto de acesso a algumas bolhas
    	
    	step = *(OApontador +j);
  		int eliminou = FALSE; //variável que indica se já foram eliminadas bolhas na coluna j
  		int i = 0; //variável que guarda a posição relativa das bolhas na coluna
  		
  		while(step != NULL)//percorre as linhas
  		{
  			if (step->bolha.marca == MARCADA_PARA_ELIMINACAO)
  			{	
  				//liberta o bloco
  		    	apaga = step;
  		    	step = step->prox;
  		    	free(apaga);
  		    	bolhas_rebentadas_total++;
  		    	eliminou = TRUE;

  		    	if (anterior == NULL)
  		      		*(OApontador +j) = step;
  		    	else
				anterior->prox = step;
  		  	}
  		  	else
  		  	{	
  		  		/*todas as bolhas acima de uma bolha eliminada serão animadas ao cair, para isso é necessário saber a posição relativa entre elas
  		  		e que se destaquem das outras marcas com o auxilio da macro ANIMACAO_DESCER*/
  		    	if (eliminou == TRUE)
  		      		step->bolha.marca = ANIMACAO_DESCER + i;

  		    	anterior = step;
  		    	step = step->prox;
  		  	}
  		  	i++;
  		}
  	}

  return bolhas_rebentadas_total;
}


/*
Função que trata da animação da implosão das bolhas
Para que as bolhas impludam todas simultaneamente em todas as iteracoes todas as bolhas são imprimidas novamente e as maracdas 
para eliminação com um raio cada vez menor
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
*/
void atualizaJanela_animacoes_implosao(DADOS dados, APBLOCO * OApontador)
{
	int iteracoes = -5; //variável responsável pela terminação do ciclo
	int raio = dados.dimBolha/2; //raio inicial

	do{//até todas as iterações terem sido feitas
		//limpa a janela
		SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255);
  		SDL_RenderClear(g_pRenderer); //limpar a janela com branco
		desenha_butoes(dados);//desneha butões e pontuação
  
  		raio -= (dados.dimBolha/2)/ITERACOES_IMPLOSAO; //raio vai diminuindo

  		for (int j = 0; j < dados.larguraJanela; ++j)//percorre todas as colunas
  		{	
  			int i = 0; //coordenada ao logo da coluna
  			APBLOCO bolhaAtual = *(OApontador +j);

    		while(bolhaAtual != NULL)
    		{
        		if (bolhaAtual == NULL)
        		  break;
        		int cor[3];//variavel que guarda a cor da bolha
        		corBolha(bolhaAtual->bolha.cor, cor);//a partir do codigo da cor que esta na matriz, da valores de rgbào vetor cor
        		 //desenha a bolha depois de transformadas as coordenas da matriz para o referencial(em pixeis) da janela
        		int pos_janela_x = CoordenadasMatriztransformaParaRefJanela(dados, j, EIXO_X);
        		int pos_janela_y = CoordenadasMatriztransformaParaRefJanela(dados, i, EIXO_Y); 
				
        		if (bolhaAtual->bolha.marca == MARCADA_PARA_ELIMINACAO) //se a bolha está marcada para implosão
        		{	
        			if (iteracoes != ITERACOES_IMPLOSAO) //na ultimaiteração não imprime nada
        			{
        				filledCircleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, raio, cor[0], cor[1], cor[2],255);
        				circleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, raio, 0, 0, 0,255);
        				SDL_Delay(DELAY_IMPLOSAO);	
        			}

        			bolhaAtual = bolhaAtual->prox;
        			i++;
 					continue;
        		}
				
				//caso a bolha não esteja marcada para eliminação
        		filledCircleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, dados.dimBolha/2.0, cor[0], cor[1], cor[2],255);
        		circleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, (dados.dimBolha/2.0), 0, 0, 0,255);
        		bolhaAtual = bolhaAtual->prox;
        		i++;
  			}
  		}
  			
  		SDL_RenderPresent(g_pRenderer); //atualiza a janela
  		iteracoes++;
	}while(iteracoes <= ITERACOES_IMPLOSAO); //enquanto todas as iterações nao tiverem sido feitas
}


//------------------------------------------------ queda de bolhas --------------------------------------------

/*
Função que trata da animação da queda de novas bolhas que substituem as eliminadas.
Assim, a função seleciona as bolhas marcadas com ANIMACAOO_DESCER + posição relativas na sua coluna imaginando-as numa nova matriz a
cima da janela, mecanismo usado para facilitar a animação da queda. 
As bolhas começam todas a uma altura de tres casas aciam do topo da janela e a altura relativ vertical vai diminuindo ao longo do 
ciclo até as bolhas atingirem a posição coorespondente às suas coordenas reais nas colunas.
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
*/
void atualizaJanela_animacoes_descer_novas(DADOS dados, APBLOCO * OApontador)
{
	int over = TRUE; //regula a execução do ciclo
	float altura = dados.alturaJanela + 2; //altura inicial em coordenadas verticais
	do
	{	
		//limpa o ecrâ
		SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255);
  		SDL_RenderClear(g_pRenderer); //limpar a janela com branco
		over = TRUE;
		desenha_butoes(dados); //desenha a pontuação e os botoes
  		altura -= DS; //ao longo das iterações a altura vai diminundo

  		for (int j = 0; j < dados.larguraJanela; ++j)//percorre todas as colunas
  		{	
  			int i = 0;
  			APBLOCO bolhaAtual = *(OApontador + j);
    		
    		while(bolhaAtual != NULL)
    		{		
        		if (bolhaAtual == NULL)
        		  	break;
        		int cor[3];//variavel que guarda a cor da bolha
        		corBolha(bolhaAtual->bolha.cor, cor);//a partir do codigo da cor que esta na matriz, da valores de rgbào vetor cor
        		 //desenha a bolha depois de transformadas as coordenas da matriz para o referencial(em pixeis) da janela
			
				//a posição em relaçaõ ao eixo x numca varia
        		int pos_janela_x = CoordenadasMatriztransformaParaRefJanela(dados, j, EIXO_X);
        		int pos_janela_y;
			
        		if (bolhaAtual->bolha.marca < ANIMACAO_DESCER) //se a bolha não estiver marcada para descer com animação
        			pos_janela_y = CoordenadasMatriztransformaParaRefJanela(dados, i, EIXO_Y); //coordenadas normais
        		else
        		{	
        			//cordenadas relativas
        			pos_janela_y = CoordenadasMatriztransformaParaRefJanela_coordenadas_continuas(dados, altura + bolhaAtual->bolha.marca- ANIMACAO_DESCER, EIXO_Y);
        			over = FALSE;
        			if (altura + bolhaAtual->bolha.marca- ANIMACAO_DESCER <= i) //se a bolha chegar ao seu sitio a sua marca deixa de ser ANIMACAO_DESCER
        				bolhaAtual->bolha.marca = SOFREU_ALTERACAO; //passa a ser sofrer alteração pois a sua descida pode ter formado novos caminhos
        		}
        		
        		//desenha as bolhas
        		filledCircleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, dados.dimBolha/2.0, cor[0], cor[1], cor[2],255);
        		circleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, (dados.dimBolha/2.0), 0, 0, 0,255);
        		bolhaAtual = bolhaAtual->prox;
        		i++;
    		}
  		}
  		SDL_RenderPresent(g_pRenderer);
  		SDL_Delay(DELAY_DESCER);

	}while(over == FALSE);
}

/*
Função que trata da animação da queda das bolhas acima de outras que tenham sido eliminadas
As bolhas tem na sua marca a sua posição anterior + ANIMACAO_DESCER ao longo deste ciclo as bolhas são desenhadas no ecrã 
até que a diferenca entre a sua posiçao anterior - "altura" e a posiçao que adotarão na dispoção de estabilidade for nula.
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
*/
void atualizaJanela_animacoes_descer_velhas(DADOS dados, APBLOCO * OApontador)
{
	int over = TRUE; //regula a execução do ciclo
	float altura = 0; //a altura relativa começa a 0
	do
	{
		//limpa a janela
		SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255);
  		SDL_RenderClear(g_pRenderer); //limpar a janela com branco
		over = TRUE;
		desenha_butoes(dados); //desenha butoes e 
  		altura -= DS;//altura diminui ao longo das iterações

  		for (int j = 0; j < dados.larguraJanela; ++j)//percorre todas as colunas
  		{	
  			int i = 0;
  			APBLOCO bolhaAtual = *(OApontador +j);
    		while(bolhaAtual != NULL)
    		{
        		int cor[3];//variavel que guarda a cor da bolha
        		corBolha(bolhaAtual->bolha.cor, cor);//a partir do codigo da cor que esta na matriz, da valores de rgbào vetor cor
   				//coordenada no eixo x permanece inalterada
        		int pos_janela_x = CoordenadasMatriztransformaParaRefJanela(dados, j, EIXO_X);
        		int pos_janela_y;
			
				//a bolha não está marcada para descer
        		if (bolhaAtual->bolha.marca < ANIMACAO_DESCER)
        			pos_janela_y = CoordenadasMatriztransformaParaRefJanela(dados, i, EIXO_Y); 
        		else
        		{
        			pos_janela_y = CoordenadasMatriztransformaParaRefJanela_coordenadas_continuas(dados, altura + bolhaAtual->bolha.marca- ANIMACAO_DESCER, EIXO_Y);
        			over = FALSE;
        			if (altura + bolhaAtual->bolha.marca - ANIMACAO_DESCER <= i) //atá as coordenasdas relativas serem as mesmas que as coordenadas normais da bolha
        				bolhaAtual->bolha.marca = SOFREU_ALTERACAO;
        		}
        		
        		//desenha a bolha
        		filledCircleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, dados.dimBolha/2.0, cor[0], cor[1], cor[2],255);
        		circleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, (dados.dimBolha/2.0), 0, 0, 0,255);
        		bolhaAtual = bolhaAtual->prox;
        		i++;
    		}
  		}
  		//atualiza a janela
  		SDL_RenderPresent(g_pRenderer);
  		SDL_Delay(DELAY_DESCER);

	}while(over == FALSE);
}



/*
Função que transforma coordenaadas contúnuas da matriz para coordenadas da janela
Esta função é necessária para calcular a posição das bolhas ao cair durante a animação de queda
Recebe o valor da coordenada e indicação de que eixo é que essa coordenada se refere (0-X, 1-Y) e retorna o valor dessa coordenada no referencial da janela
*/
int CoordenadasMatriztransformaParaRefJanela_coordenadas_continuas(DADOS dados, float coordenada,  int eixo)
{
  if (eixo == EIXO_X)
    return (int) (coordenada+1)*(dados.dl)*dados.dimBolha + (dados.dimBolha/2.0)*(2*coordenada+1); //equacao para os xx

  else if (eixo == EIXO_Y)
    return (int) (dados.windowSizeY - ((coordenada +1)*(dados.dl)*dados.dimBolha + (dados.dimBolha/2.0)*(2*coordenada+1) + 
    			1.5*dados.dimBolha ) + dados.altura_barra_superior ); // eqaucao para os yy

  return 0;
}

//----------------------------------------------- troca de bolhas -----------------------------------------------

/* 
Função que verifica em que bolha o utilizador carregou se é que o fez
No caso de ter caregado numa bolha, marca-a para para ser animada para uam troca
Recebe como argumentos: os dados do jogo
						o apontador de acesso à informação referente às bolhas
Retorna: FALSE se nao carregou em nenhuma bolha
         TRUE se carregou numa bolha e a marcou com sucesso
*/
int deteta_clique_troca_fixe(DADOS dados, APBLOCO * OApontador, TROCA_FIXE_INFO * troca_info)
{
	int mouse_pos[2];
  	SDL_GetMouseState(&mouse_pos[0], &mouse_pos[1]); //deteta a posição do rato

  	for (int j = 0; j < dados.larguraJanela; ++j) //varre todas as colunas
  	{
    	int i = 0;
    	APBLOCO bolha_carregada = *(OApontador + j);
    	//Corre todas as bolhas em jogo até encontrar uma cujo centro diste de menos de um raio em relação ao sitio onde o utilizador carregou
    	while(bolha_carregada != NULL)
    	{
     		int centro_x = CoordenadasMatriztransformaParaRefJanela(dados, j,  EIXO_X);
      		int centro_y = CoordenadasMatriztransformaParaRefJanela(dados, i,  EIXO_Y);
      
      		if (bolha_carregada != NULL && distancia(centro_x, centro_y, mouse_pos[0], mouse_pos[1]) <= dados.dimBolha/2) //se a distancia for inferior a um raio
      		{	 
      			//preenche a informação acerca da troca
        		troca_info->x0 = centro_x; 
       			troca_info->y0 = centro_y;
          		troca_info->i = i;
          		troca_info->j = j;
          		troca_info->direcao = 0;
          		bolha_carregada->bolha.marca = ANIMACAO_TROCA;
          		return TRUE;
      		}

      		bolha_carregada = bolha_carregada->prox;
      		i++;
    	}
  	}
  return FALSE;
}

/*
Função que define a direção da troca (1 - horizontal e 2 - vertical)
Recebe por endereco a estrutura com a informação da troca
Define a direção da troca como sendo a direção em que o rato mais se movimentou na primeira vez em que se afastou do ponto em que inicialmete carregou
*/
void define_direcao(TROCA_FIXE_INFO * troca_info)
{
	int mouse_pos[2];
  	SDL_GetMouseState(&mouse_pos[0], &mouse_pos[1]); //deteta a posição do rato

  	int dx = abs(mouse_pos[0]-troca_info->x0); //distancia horizintal
  	int dy = abs(mouse_pos[1]-troca_info->y0); //distancia veritical

  	if (dx > dy) //o deslocamento horizontal é maior
  		troca_info->direcao = 1;
  	else
  		troca_info->direcao = 2;

}

/*
Função que à medida que o rato vai mexendo movimenta as bolhas para que a troca seja visualmente mais apelativa
Assim, por forma a passar a informação da magnitude e direção do desvio "codifica-o" na marca da bolha através da diferença em relação à macro
ANIMACAO_TROCA
Recebe: os dados do jogo
		O apontador de acesso a toda a informação do programa
		a informação da troca da bolha 
*/
void mexe_bolhas_troca(DADOS dados, APBLOCO * OApontador, TROCA_FIXE_INFO troca_info)
{
	int mouse_pos[2];
  	SDL_GetMouseState(&mouse_pos[0], &mouse_pos[1]); //deteta a posição do rato

  	int ds; //variável que condicionará a disância das bolhas à sua posição usual

  	if (troca_info.direcao == 1) //se a troca é horizontal
  		ds = mouse_pos[0]-troca_info.x0; 
  	else
  		ds = -(mouse_pos[1]-troca_info.y0); //refrencial invertido nos yy implica a utilização de -


  	restringe_ds(dados, &ds); //restringe os valores de ds para que a bolha não se movimente mais do que para as posições adjacentes

  	APBLOCO bolhaTroca1, bolhaTroca2, bolhaNaoTroca;
  	
  	//analisa os quatro casos separadamente
  	if (ds >= 0 && troca_info.direcao == 1)
  	{	
  		bolhaTroca1 = apbloco_bolha_em_i_j(OApontador, troca_info.i, troca_info.j, dados);
  		bolhaTroca2 = apbloco_bolha_em_i_j(OApontador, troca_info.i, troca_info.j + 1, dados);
  		bolhaNaoTroca = apbloco_bolha_em_i_j(OApontador, troca_info.i, troca_info.j - 1, dados); 

  		if (bolhaNaoTroca != NULL) //evitar que o nosso amigo apareça
  			bolhaNaoTroca->bolha.marca = 0; //reset ao desvio da bolha oposta à troca 

  		bolhaTroca1->bolha.marca = ANIMACAO_TROCA + ds; //a informação do desvia em relação à posição usual é marcada da seguinte forma

  		if (bolhaTroca2 != NULL) //evitar que o nosso amigo apareça
  			bolhaTroca2->bolha.marca = ANIMACAO_TROCA - ds;
  	}
  	else if (ds < 0 && troca_info.direcao == 1)
  	{	
  		bolhaTroca1 = apbloco_bolha_em_i_j(OApontador, troca_info.i, troca_info.j, dados);
  		bolhaTroca2 = apbloco_bolha_em_i_j(OApontador, troca_info.i, troca_info.j - 1, dados);
  		bolhaNaoTroca = apbloco_bolha_em_i_j(OApontador, troca_info.i, troca_info.j + 1, dados);

  		if (bolhaNaoTroca != NULL)
  			bolhaNaoTroca->bolha.marca = 0;

  		bolhaTroca1->bolha.marca = ANIMACAO_TROCA + ds;

  		if (bolhaTroca2 != NULL)
  			bolhaTroca2->bolha.marca = ANIMACAO_TROCA - ds;
  	}
  	else if (ds >= 0 && troca_info.direcao == 2)
  	{
  		bolhaTroca1 = apbloco_bolha_em_i_j(OApontador, troca_info.i, troca_info.j, dados);
  		bolhaTroca2 = apbloco_bolha_em_i_j(OApontador, troca_info.i + 1, troca_info.j, dados);
  		bolhaNaoTroca = apbloco_bolha_em_i_j(OApontador, troca_info.i - 1, troca_info.j, dados);

  		if (bolhaNaoTroca != NULL)
  			bolhaNaoTroca->bolha.marca = 0;

  		bolhaTroca1->bolha.marca = ANIMACAO_TROCA - ds;

  		if (bolhaTroca2 != NULL)
  			bolhaTroca2->bolha.marca = ANIMACAO_TROCA + ds;

  	}
  	else if (ds < 0 && troca_info.direcao == 2)
  	{
  		bolhaTroca1 = apbloco_bolha_em_i_j(OApontador, troca_info.i, troca_info.j, dados);
  		bolhaTroca2 = apbloco_bolha_em_i_j(OApontador, troca_info.i - 1, troca_info.j, dados);
  		bolhaNaoTroca = apbloco_bolha_em_i_j(OApontador, troca_info.i + 1, troca_info.j, dados);

  		if (bolhaNaoTroca != NULL)
  			bolhaNaoTroca->bolha.marca = 0;

  		bolhaTroca1->bolha.marca = ANIMACAO_TROCA - ds;
  		if (bolhaTroca2 != NULL)
  			bolhaTroca2->bolha.marca = ANIMACAO_TROCA + ds;
  	}

  	atualizaJanela_animacao_troca(dados, OApontador, troca_info); //desenha as bolhas com os desvios
}

/*
Função que restringe o valor de ds, de modo a que os devios não sejam superiores a uma bolha e um itervalo
Assim, recebe, para além dos dados, ds por endereço e restringe-o se necessário
*/
void restringe_ds(DADOS dados, int * ds)
{
	if (abs(*ds) > dados.dimBolha + dados.dl*dados.dimBolha) //se o desvio for superior ao desejado
	{
		if(*ds > 0)
			*ds = dados.dimBolha + dados.dl*dados.dimBolha; //é atribuido o valor máximo
		else
			*ds = -(dados.dimBolha + dados.dl*dados.dimBolha);
	}
}

/*
Função que trata do desenho da animação da troca das bolhas
Assim, desenha as bolhas com os devios indicados nas suas marcas
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
        a informação da troca
*/
void atualizaJanela_animacao_troca(DADOS dados, APBLOCO * OApontador, TROCA_FIXE_INFO troca_info)
{
	//limpa o ecrâ
	SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255);
  	SDL_RenderClear(g_pRenderer); //limpar a janela com branco
	desenha_butoes(dados); //desenha a pontuação e os botoes
  	for (int j = 0; j < dados.larguraJanela; ++j)//percorre todas as colunas
  	{	
  		int i = 0;
  		APBLOCO bolhaAtual = *(OApontador + j);
    	
    	while(bolhaAtual != NULL)
    	{		
       		int cor[3];//variavel que guarda a cor da bolha
       		corBolha(bolhaAtual->bolha.cor, cor);//a partir do codigo da cor que esta na matriz, da valores de rgbào vetor cor
       		 //desenha a bolha depois de transformadas as coordenas da matriz para o referencial(em pixeis) da janela
       		int pos_janela_x = CoordenadasMatriztransformaParaRefJanela(dados, j, EIXO_X);
       		int pos_janela_y = CoordenadasMatriztransformaParaRefJanela(dados, i, EIXO_Y);
			
       		if(bolhaAtual->bolha.marca >= ANIMACAO_TROCA - 100) //se estiver marcada para a aanimação
       		{	
       			if (troca_info.direcao == 1) //troca na horizontal
       				pos_janela_x += bolhaAtual->bolha.marca - ANIMACAO_TROCA;
       			if (troca_info.direcao == 2) //troca na vertical
       				pos_janela_y += bolhaAtual->bolha.marca - ANIMACAO_TROCA;
       		}
       			
       		//desenha as bolhas
       		filledCircleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, dados.dimBolha/2.0, cor[0], cor[1], cor[2],255);
        	circleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, (dados.dimBolha/2.0), 0, 0, 0,255);
       		bolhaAtual = bolhaAtual->prox;
       		i++;	
    	}
	}
	SDL_RenderPresent(g_pRenderer); //atualiza o renderer
}


/*
Função que após a animação das bolhas ter terminado avalia as seus devios e caso sejam grandes o suficentes troca as bolhas
Recebe: os dados do jogo
		O apontador de acesso a toda a informação relativa às bolhas
*/
void normaliza_bolhas_trocadas_animacao(DADOS dados, APBLOCO * OApontador)
{
	int i; //variavel para correr as colunas
	int j; //variavel para correr as colunas
	int BREAK = FALSE; //flag para controlar o ciclo

	APBLOCO bolhaTroca_1; //apontador para o bloco de uma das bolhas a ser trocadas

	for (j = 0; j < dados.larguraJanela; ++j)//percorre as linhas
  	{
   		bolhaTroca_1 = *(OApontador + j); //no inicio de cada coluna o apontador está no vetor dinâmico
    	i = 0;

    	while(bolhaTroca_1 != NULL)//percorre as colunas
    	{
     		if (bolhaTroca_1->bolha.marca != 0) //quando for encontrada a primeira bolha para troca sai do ciclo
     		{
     			if (abs(bolhaTroca_1->bolha.marca - ANIMACAO_TROCA) >= (int) (0.51*dados.dimBolha) ) //se o desvio for grande o suficiente
     			{
     				BREAK = TRUE; //para sair dos dois ciclos usa a flag BREAK
     				break;
     			}
     		}
      		bolhaTroca_1 = bolhaTroca_1->prox;
      		i++;
    	}	

    	if (BREAK == TRUE) //se a flag for TRUE sai do ciclo
     		break;
  	}

  	/* Sabendo que apenas duas bolhas podem estar marcadas e que o algoritmo de procura utilizado corre a janela da esquerda para a direita e debaixo para cima,
  	há três possiblidades:
   	 	-> a segunda bolha a trocar está à direita e terá de ser feita uma troca entre colunas
    	-> a segunda bolha a trocar está a cima e terá de ser feita uma troca na mesma coluna
    	-> a segunda bolha a trocar não está em nenhuma das situações anteriores pelo que bolha está situada numa borda da janela
  	Assim, trataremos as duas primeiras possibilidades e no caso da terceira eliminaremos apenas a marca da bolha
  	*/

  	//variáveis que guardarão os endereços das bolhas a cima e à direita da primeira identificada para a troca
  	if (bolhaTroca_1 == NULL) //se nenhuma bolha for trocada
  	{	
    	neutraliza_marcas_animacao_troca(dados, OApontador); //elimina as marcas de animação
  		reset(dados, OApontador); //imprime alterações
  		return;
  	}

  	//teste Às bolhas a cima e à direita
  	APBLOCO bolha_a_cima = bolhaTroca_1->prox;
  	APBLOCO bolha_direita = apbloco_bolha_em_i_j(OApontador, i,j+1, dados);


  	if (bolha_a_cima!= NULL && bolha_a_cima->bolha.marca >= ANIMACAO_TROCA - 100) //a bolha a cima foi marcada para troca
  	{
    	APBLOCO anterior = apbloco_bolha_em_i_j(OApontador, i-1, j, dados); //apontador para a bolha anterior necessária para a função que fará a troca
  		troca_em_coluna(OApontador, anterior, bolhaTroca_1, j);//troca na mesma coluna
  	}
  	else if(bolha_direita != NULL && bolha_direita->bolha.marca >= ANIMACAO_TROCA - 100)
    	troca_entre_colunas(OApontador, dados, bolhaTroca_1, i, j);	//troca bolhas entre colunas

    neutraliza_marcas_animacao_troca(dados, OApontador);//elimina as marcas de animação
    reset(dados, OApontador);
}

/*
Função que corre todas as bolhas do jogo e anula a marca de todas as bolhas que estejam marcadas para eliminação temporária
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
*/
void neutraliza_marcas_animacao_troca(DADOS dados, APBLOCO * OApontador)
{
  for (int j = 0; j < dados.larguraJanela; ++j)//percorre as linhas
  {
    APBLOCO bolhaAtual = *(OApontador +j);
    while(bolhaAtual != NULL)
    {
      if(bolhaAtual->bolha.marca != SOFREU_ALTERACAO) //se encontrar uma bolha que não esteja marcada como tendo sofrido alteracao
        bolhaAtual->bolha.marca = 0;//anula a marca

      bolhaAtual = bolhaAtual->prox;
    }
  }
}




