/*
Ficheiro graphics.c
Este é o ficheiro onde estão definidas todas as funções que contribuem para a atualização da compponente gráfica do jogo
Estão também incluidas neste ficheiro as funções que permitem iniciar e encerrar a janela, desenhar as marcas de troca e
eliminação das bolhas e detetar a posição do clique do jogador.
*/

//----------------------------------------------------------------------INCLUDES----------------------------------------------------------------------
//incluir o header com protótipos das funções, macros e outros ficheiros com funções que necessitaram de ser usadas
#include "graphics.h" 

//--------------------------------------------------------------------VARIÁVEIS GLOBAIS----------------------------------------------------------------
//variáveis de acesso à janela e ao renderer
SDL_Window* g_pWindow = NULL; 
SDL_Renderer* g_pRenderer = NULL;

//----------------------------------------------------------------------FUNÇÕES----------------------------------------------------------------------

//----------------------------------------controlo------------------------------------------
/*
Função que inicia a bibioteca gráfica e que abre a janela que servirá de suporte ao jogo
Recebe: os dados do programa para poder dimensionar corretamente a janela
*/
void iniciarSDL2(DADOS dados)
{
  if(SDL_Init(SDL_INIT_EVERYTHING) >= 0) 
  {
    //criar a janela
    g_pWindow = SDL_CreateWindow("Bubble Trouble", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dados.windowSizeX, dados.windowSizeY,SDL_WINDOW_SHOWN);
    //criar o renderer se a janela foi aberta com sucesso
    if(g_pWindow != 0) 
      g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
    else 
    {
      printf("Erro na iniciação da biblioteca gráfica\n");
      exit(0);
    } 
  }
  else
  {
    printf("Erro na iniciação da biblioteca gráfica\n");
    exit(0);
  } 
  SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255); //pintar o background com branco
  SDL_RenderClear(g_pRenderer); //limpar a janela com branco
}

/*
Função que limpa a janela e a desenha já com as alterações feitas posteriormente
Recebe: os dados do jogo
        o apontador de acesso a toda a informação relativa às bolhas
*/
void reset(DADOS dados, APBLOCO * OApontador)
{ 
  SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255);
  SDL_RenderClear(g_pRenderer); //limpar a janela com branco
  atualizaJanela(dados, OApontador); //desenha as bolhas na janela já atualizadas
  SDL_RenderPresent(g_pRenderer); //atualiza o renderer
}

/*
Função que termina a utilização da biblioteca gáfica destruindo o renderer e a janela
*/
void sairSDL2()
{
  SDL_DestroyWindow(g_pWindow);
  SDL_HideWindow(g_pWindow);
  SDL_DestroyRenderer(g_pRenderer);
  SDL_Quit();
}


//----------------------------------------desenho------------------------------------------


/*
Função que desenha os botões e as bolhas em jogo
Recebe: os dados do jogo
        o apontador de acesso a toda a informação relativa às bolhas
*/
void atualizaJanela(DADOS dados, APBLOCO * OApontador)
{
  
  desenha_butoes(dados); //desenha butões e pontuação
  for (int j = 0; j < dados.larguraJanela; ++j)//todas as colunas
  {
    
    APBLOCO bolhaAtual = *(OApontador + j);
    int i = 0; //coordenada vertical

    while(bolhaAtual != NULL)
    {
      if (bolhaAtual == NULL)
        break;
      int cor[3]; //variavel que guarda a cor da bolha
      corBolha(bolhaAtual->bolha.cor, cor);//a partir do codigo da cor que esta na matriz, dá valores de rgb vetor cor
      
      //desenha a bolha depois de transformadas as coordenas da matriz para o referencial (em pixeis) da janela
      int pos_janela_x = CoordenadasMatriztransformaParaRefJanela(dados, j, EIXO_X);
      int pos_janela_y = CoordenadasMatriztransformaParaRefJanela(dados, i, EIXO_Y);    
      filledCircleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, dados.dimBolha/2.0, cor[0], cor[1], cor[2],255);
      circleRGBA(g_pRenderer, pos_janela_x, pos_janela_y, (dados.dimBolha/2.0), 0, 0, 0,255);
      
      if (bolhaAtual->bolha.marca == MARCADA_PARA_TROCA) //as bolhas que seão trocadas são marcadas com um mais
      	pinta_mais(pos_janela_x, pos_janela_y, dados, OApontador);
      //no modo de debug as bolhas que serão eliminadas serão pintadas com um X
      else if(dados.debug == TRUE && bolhaAtual->bolha.marca == MARCADA_PARA_ELIMINACAO) 
        pinta_X(pos_janela_x, pos_janela_y, dados, OApontador);

      bolhaAtual = bolhaAtual->prox;
      i++;
    }
  }
}

/*
Função que desenha os butões e pontuação do jogador na janela
Recebe: os dados do programa
*/
void desenha_butoes(DADOS dados)
{
  SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 205, 255); //selecionar a cor azul para desenhar linhas
  char strPontos[20]=""; //string para imprimir no ecra a pontuação
  SDL_RenderDrawLine( g_pRenderer,0, dados.altura_barra_superior, dados.windowSizeX, dados.altura_barra_superior); //desenhar a linha superiror
  //Botao novo jogo
  SDL_RenderDrawLine( g_pRenderer,0.02*dados.windowSizeX, 0.25*dados.dimBolha, 0.20*dados.windowSizeX, 0.25*dados.dimBolha);
  SDL_RenderDrawLine( g_pRenderer,0.02*dados.windowSizeX, 0.25*dados.dimBolha, 0.02*dados.windowSizeX, 1.25*dados.dimBolha);
  SDL_RenderDrawLine( g_pRenderer,0.02*dados.windowSizeX, 1.25*dados.dimBolha, 0.20*dados.windowSizeX, 1.25*dados.dimBolha);
  SDL_RenderDrawLine( g_pRenderer,0.20*dados.windowSizeX, 0.25*dados.dimBolha, 0.20*dados.windowSizeX, 1.25*dados.dimBolha);
  if(dados.windowSizeX > LARGURA_MINIMA1) //escreve novo jogo numa linha se a largura da janela for superior a um determinado valor critico
    stringRGBA(g_pRenderer, 0.11*dados.windowSizeX-37, 0.75*dados.dimBolha-4, "Novo Jogo", 0,0,0, 255);
  else //se a largura for inferior a um valor critico imprime as duas palavras na vertical
  {
    stringRGBA(g_pRenderer, 0.20*dados.windowSizeX-30, 0.50*dados.dimBolha-4, "Novo", 0,0,0, 255);
    stringRGBA(g_pRenderer, 0.20*dados.windowSizeX-30, 0.90*dados.dimBolha-4, "Jogo", 0,0,0, 255);
  }

  //desenha o botao fim
  SDL_RenderDrawLine( g_pRenderer,0.25*dados.windowSizeX, 0.25*dados.dimBolha, 0.37*dados.windowSizeX, 0.25*dados.dimBolha);
  SDL_RenderDrawLine( g_pRenderer,0.25*dados.windowSizeX, 0.25*dados.dimBolha, 0.25*dados.windowSizeX, 1.25*dados.dimBolha);
  SDL_RenderDrawLine( g_pRenderer,0.25*dados.windowSizeX, 1.25*dados.dimBolha, 0.37*dados.windowSizeX, 1.25*dados.dimBolha);
  SDL_RenderDrawLine( g_pRenderer,0.37*dados.windowSizeX, 0.25*dados.dimBolha, 0.37*dados.windowSizeX, 1.25*dados.dimBolha);
  stringRGBA(g_pRenderer, 0.31*dados.windowSizeX-12, 0.75*dados.dimBolha-4, "Fim", 0,0,0, 255);

  // desenha o quadro da pontucao e imprime os pontos do jogador
  SDL_RenderDrawLine( g_pRenderer,0.60*dados.windowSizeX, 0.25*dados.dimBolha, 0.98*dados.windowSizeX, 0.25*dados.dimBolha);
  SDL_RenderDrawLine( g_pRenderer,0.60*dados.windowSizeX, 0.25*dados.dimBolha, 0.60*dados.windowSizeX, 1.25*dados.dimBolha);
  SDL_RenderDrawLine( g_pRenderer,0.60*dados.windowSizeX, 1.25*dados.dimBolha, 0.98*dados.windowSizeX, 1.25*dados.dimBolha);
  SDL_RenderDrawLine( g_pRenderer,0.98*dados.windowSizeX, 0.25*dados.dimBolha, 0.98*dados.windowSizeX, 1.25*dados.dimBolha);
  stringRGBA(g_pRenderer, dados.windowSizeX*0.62, 0.75*dados.dimBolha-4, "Pontos:", 0,0,0, 255);
  sprintf(strPontos, "%d", dados.pontos); //imprime numa string os pontos do utilizador
  stringRGBA(g_pRenderer, dados.windowSizeX*0.62 + 56, 0.75*dados.dimBolha-4, strPontos, 0,0,0, 255); //imprime os pontos
}

/*
Função que desenha um + na bolha que cujas coordenadas recebe
Recebe: as coordenas da bolha onde é para imprimir o +, mais_x e mais_y
        os dados do programa
        o apontador de acesso a toda a informação relativa às bolhas
*/
void pinta_mais(int mais_x, int mais_y, DADOS dados, APBLOCO * OApontador)
{
	SDL_SetRenderDrawColor(g_pRenderer, 255, 179, 0, 255);
	SDL_RenderDrawLine(g_pRenderer, mais_x - (int) (0.6 * dados.dimBolha/2), mais_y, mais_x + (int) (0.6 * dados.dimBolha/2),  mais_y);
	SDL_RenderDrawLine(g_pRenderer, mais_x,  mais_y-(int) (0.6 * dados.dimBolha/2), mais_x, mais_y+(int) (0.6 * dados.dimBolha/2));	
}

/*
Função que desenha um X na bolha que cujas coordenadas recebe
Recebe: as coordenas da bolha onde é para imprimir o +, mais_x e mais_y
        os dados do programa
        o apontador de acesso a toda a informação relativa às bolhas
*/
void pinta_X(int mais_x, int mais_y, DADOS dados, APBLOCO * OApontador )
{
  SDL_SetRenderDrawColor(g_pRenderer, 255, 179, 0, 255);
  SDL_RenderDrawLine(g_pRenderer, mais_x - (int) (0.6 * dados.dimBolha/2 * cos(PI/4)), mais_y - (int) (0.6 * dados.dimBolha/2 * sin(PI/4)), 
                                  mais_x + (int) (0.6 * dados.dimBolha/2 * cos(PI/4)), mais_y + (int) (0.6 * dados.dimBolha/2 * sin(PI/4)));
  SDL_RenderDrawLine(g_pRenderer, mais_x - (int) (0.6 * dados.dimBolha/2 * cos(PI/4)), mais_y + (int) (0.6 * dados.dimBolha/2 * sin(PI/4)), 
                                  mais_x + (int) (0.6 * dados.dimBolha/2 * cos(PI/4)), mais_y - (int) (0.6 * dados.dimBolha/2 * sin(PI/4)));
}


//----------------------------------------posição na janela e deteção de clique------------------------------------------

/*
Função que verifica se o utilizador carregou nos botoes ou nas bolhas
No caso de ter caregado numa bolha, marca-a para troca se não estiver marcada, caso contrário desmarca-a e retorna -1
Retorna: 0 se nao carregou em nada
         3 se carrega no botao fim do jogo
         4 se carrega no botao novo jogo
         1 se carregou numa bolha e a marcou com sucesso
         -1 se carregou na bolha que estava já marcada
*/
int deteta_clique(DADOS dados, APBLOCO * OApontador)
{

  int mouse_pos[2];
  SDL_GetMouseState(&mouse_pos[0], &mouse_pos[1]); //deteta a posição do rato
  if(mouse_pos[0]>= 0.02*dados.windowSizeX && mouse_pos[0]<=0.20*dados.windowSizeX && 
      mouse_pos[1]>= 0.25*dados.dimBolha && mouse_pos[1]<=1.25*dados.dimBolha) //limites do botao novo jogo
    return 4;
  else if(mouse_pos[0]>= 0.25*dados.windowSizeX && mouse_pos[0]<=0.37*dados.windowSizeX && 
        mouse_pos[1]>= 0.25*dados.dimBolha && mouse_pos[1]<=1.25*dados.dimBolha) //linmites do botao fim do jogo
    return 3;

  for (int j = 0; j < dados.larguraJanela; ++j)
  {
    int i = 0;
    APBLOCO bolha_carregada = *(OApontador + j);
    //Corre todas as bolhas em jogo até encontrar uma cujo centro diste de menos de um raio em relação ao sitio onde o utilizador carregou
    while(bolha_carregada != NULL)
    {
      int centro_x = CoordenadasMatriztransformaParaRefJanela(dados, j,  EIXO_X);
      int centro_y = CoordenadasMatriztransformaParaRefJanela(dados, i,  EIXO_Y);
      
      if (dados.fixe == FALSE && bolha_carregada != NULL && distancia(centro_x, centro_y, mouse_pos[0], mouse_pos[1]) <= dados.dimBolha/2) //se a distancia for inferior a um raio
      { 
        if (bolha_carregada->bolha.marca != MARCADA_PARA_TROCA)
        {
          bolha_carregada->bolha.marca = MARCADA_PARA_TROCA; //marca para troca
          return 1;
        }
        else
        {
          bolha_carregada->bolha.marca = 0; // a bolha encontrada estava já marcada
          return -1;
        }
          
      }

      bolha_carregada = bolha_carregada->prox;
      i++;
    }
  }
  return 0;
}

/*
Função que recebe o uma cor sob a forma de um codiogo (1-9) e converte esse codigo para rgb que atribui ao vetor cor que recebe
*/
void corBolha(int codigoCor, int cor[])
{
  switch(codigoCor) //valor do codigo de cor
  {
    case 1: //vermelho
      cor[0]=255;
      cor[1]=0;
      cor[2]=0;
      break;
    case 3://azul
      cor[0]=0;
      cor[1]=0;
      cor[2]=255;
      break;
    case 2://roxo
      cor[0]=128;
      cor[1]=0;
      cor[2]=128;
      break;
    case 4://cyan
      cor[0]=0;
      cor[1]=255;
      cor[2]=255;
      break;
    case 5://verde
      cor[0]=50;
      cor[1]=205;
      cor[2]=50;
      break;
    case 6://amarelo
      cor[0]=255;
      cor[1]=255;
      cor[2]=0;
      break;
    case 7: //castanho
      cor[0]=139;
      cor[1]=69;
      cor[2]=19;
      break;
    case 8: //preto
      cor[0]=0;
      cor[1]=0;
      cor[2]=0;
      break;
    case 9: //branco
      cor[0]=255;
      cor[1]=255;
      cor[2]=255;
      break;
  }
}

/*
Função que transforma coordenas da matriz para coordenadas da janela
Recebe o valor da coordenada e indicação de que eixo é que essa coordenada se refere (X, Y) e retorna o valor dessa coordenada no referencial da janela
*/
int CoordenadasMatriztransformaParaRefJanela( DADOS dados, int coordenada, int eixo)
{
  if (eixo == EIXO_X)
    return (int) (coordenada+1)*(dados.dl)*dados.dimBolha + (dados.dimBolha/2.0)*(2*coordenada+1); //equacao para os xx
  else if (eixo == EIXO_Y)
    return (int) (dados.windowSizeY - ((coordenada +1)*(dados.dl)*dados.dimBolha + (dados.dimBolha/2.0)*(2*coordenada+1) 
                    + 1.5*dados.dimBolha ) + dados.altura_barra_superior ); // eqaucao para os yy
  return 0;  
}

/*
Função que recebe as coordenas de dois pontos num referencial cartesiano e retorna a distância entre eles
*/
float distancia(int x1, int y1 , int x2, int y2)
{

  return sqrt( (float)(x1-x2)*(x1-x2) + (float)(y1-y2)*(y1-y2));
}

