/*
Ficheiro graphics.c
Este é o ficheiro onde estão definas as funções relacionas com a leitura dos dados introduzidos pelo jogador no terminal
*/

//----------------------------------------------------------------------INCLUDES----------------------------------------------------------------------
//incluir o header com protótipos das funções, macros e outros ficheiros com funções que necessitaram de ser usadas
#include "leitura_dados.h" 

//----------------------------------------------------------------------FUNÇÕES----------------------------------------------------------------------

/*
Função que lê os dados introduzidos pelo utilizador a linha de comandos e guarda os dados na estrutura apropriada
É também responsável pela verificação de que os dados introduzidos são válidos.
Esta função recebe, por endereco, a variável que contem os dados do programa, o numero de argumentos recebidos, e o vetor de apontadores para as strings 
introduzidas
Se os dados forem inválidos a função termina o programa
*/
int lerConfigDados(DADOS * dados, int argc, char const * argv[])
{ 
  //inicializar valores
  dados->larguraJanela = 0;
  dados->alturaJanela = 0;
  dados->dimBolha = 0;
  dados->debug = FALSE;
  dados->fixe = FALSE;

  for (int i = 1; i < argc; ++i)
  {
    if(strcmp(argv[i], "-l") == 0) //ler largura da janela em bolhas
    {
      if(i+1 < argc && sscanf(argv[i+1], "%d", &(dados->larguraJanela)) != 1) 
        return 1;
      i++;
    }
    else if(strcmp(argv[i], "-d") == 0) //modo de debugging
    { 
      dados->debug = TRUE;
    }
    else if(strcmp(argv[i], "-b") == 0) //diametro da bolha em pixeis
    {
      if(i+1 < argc && sscanf(argv[i+1], "%d", &(dados->dimBolha)) != 1) 
        return 1;
      i++;
    }
    else if(strcmp(argv[i], "-f") == 0) //modo fixe
    {
      dados->fixe = TRUE;
    }
    else if(strcmp(argv[i], "-a") == 0) //ler altura da janela em bolhas
    { 
      if(i+1 < argc && sscanf(argv[i+1], "%d", &(dados->alturaJanela)) != 1) 
        return 1;
      i++;
    }
    else
    {
      printf("Erro: flag '%s' não definida.\n", argv[i]); //se o utilizador escreveu uma flag que não as acima
      return 1;
      exit(0);
    }
  }

  //verificar que todos os valores foram lidos
  if(dados->larguraJanela == 0 || dados->alturaJanela ==  0 || dados->dimBolha == 0) 
    return 1;

  //inicializar variáveis restantes
  dados->dl = DL;
  dados->pontos = 0;
  dados->altura_barra_superior = 1.5*(dados->dimBolha);
  dados->windowSizeX = round(dados->dimBolha * dados->dl * ((dados->larguraJanela) +1) + dados->larguraJanela * dados->dimBolha); //calcula as dimensoes x e y da janela 
  dados->windowSizeY= round(dados->dimBolha * dados->dl * ((dados->alturaJanela) +1) + dados->alturaJanela * dados->dimBolha + dados->altura_barra_superior);

  //----------------------------------------------------limitar valores----------------------------------------------
  if(dados->larguraJanela>= 100 || dados->alturaJanela>=100){
    printf("Erro: Número excessivo de bolhas\n");
    exit(0);
  }
  else if(dados->larguraJanela<= 0 || dados->alturaJanela<=0 || dados->dimBolha <= 0){
    printf("Erro: um ou mais dados adotam valores inválidos (nulos ou negativos).\n");
    exit(0);
  }
  else if(dados->dimBolha>50){
    printf("Erro: o raio da bolha não pode ser superior a 50px.\n");
    exit(0);
  }
  else if(dados->windowSizeX < LARGURA_MINIMA) {
    printf("Erro: Janela demasiado pequena, aumente o valor de alguns dos parametros\n");
    exit(0);
  }
  else if(dados->windowSizeY > ALTURA_MAXIMA) {
    printf("Erro: Janela demasiado grande, diminua o valor de alguns dos parametros\n");
    exit(0);
  }
  else if(dados->windowSizeX > LARGURA_MAXIMA) {
    printf("Erro: Janela demasiado grande, diminua o valor de alguns dos parametros\n");
    exit(0);
  }
    //seed para aleatórios
  if (dados->debug == TRUE) 
    srand(73);
  else 
    srand(time(NULL));

	return 0;
}


/*
Função que imprime uma mensagem de erro no caso de os dados term sido mal introduzidos pelo utilizador no terminal
*/
void mensagemErroDados()
{
  printf("\nDados inválidos.\n\n");
  printf("Ao correr o programa introduza os dados da seguinte forma:\n");
  printf("troca _bolhas -l 13 -a 13 -b 45 -f\n");
  printf("Não necessariamente nesta ordem :)\n");
  printf("Para uma melhor experiencia recomenda-se o modo fixe! (flag -f)\n");
  printf("Neste modo a troca das bolhas é feita arrastando-as!\n");
  exit(0);
}
