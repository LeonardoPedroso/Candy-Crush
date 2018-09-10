/*
Ficheiro resultados.c
Este é o ficheiro onde estão definidas todas as funções que são utilizadas para guardar a pontuação do jogador no ficheiro "resultados.txt"
*/

//----------------------------------------------------------------------INCLUDES----------------------------------------------------------------------
#include "resultados.h"

//----------------------------------------------------------------------FUNÇÕES----------------------------------------------------------------------
/*
Funcao que no final do jogo pede o nome ao utilizador e guarda o seu nome associado à pontuação obtida no jogo no ficheiro resultados.txt
Esta função está tambem responsável pela ordenação do ficheiro de resultados, fazendo uso de outras funções
Recebe como argumento os pontos que o utilizador obteve no jogo em questão
A função sai do programa em caso de invalidade do ficheiro de resultados.
*/
void fimDoJogo(int pontos)
{
  char nomeDoJogador[22]= ""; //string que reseberá o nome do utilizador
  printf("Introduza o nome do jogador (max 20 ch): " );
  fflush(stdout);
  if(scanf("%20s", nomeDoJogador)!=1) //extrai a string com o nome do jogador
  {
    printf("Erro: leitura do nome do jogador não foi efetuada com sucesso.\n");
    exit(0);
  }
  
  int pontosFichResultados[MAX_DIM_MATRIZ_RESULTADOS]; //vetor que guarda a extração dos dados do ficheiro de resultados
  char nomeDoJogadorFichResultados[MAX_DIM_MATRIZ_RESULTADOS][23]; //vetor de strings (matriz de caracteres) que guarda o nome do jogador
  char str[51] = ""; //string que será utilizada para ler as linhas e extrair informação do ficheiro de configurações
  int linha = 0;

  FILE *ficheiroResultados;
  ficheiroResultados = fopen("resultados.txt", "r"); //abre o ficheiro dos resultados no final e em modo leitura

  if (ficheiroResultados == NULL) //se há um erro na abertura do ficheiro (inexistencia na diretória do progrma)
  {
    printf("Erro: ficheiro resultados não encontrado\n");
    FILE *novoFicheiro; //cria um novo ficheiro
    novoFicheiro = fopen("resultados.txt", "w");
    fprintf(novoFicheiro, "Jogadores            Pontuação\n"); //imprimir o cabeçalho no ficheiro
    fclose(novoFicheiro);
    printf("Novo ficheiro criado...\n");   
  }
  else
  {
    fgets(str, 50, ficheiroResultados);//ignora primeira e segunda linhas
    fgets(str, 50, ficheiroResultados);
    do
    { 
      str[0] = '\0';
      fgets(str, 50, ficheiroResultados); //extrai uma linha
      if(sscanf(str, "%20s %d", nomeDoJogadorFichResultados[linha], &pontosFichResultados[linha]) != 2) //intrepreta a linha de onde extrai o nome e a pontuação
      {
        //se o numero de conversoes não for dois algo correu mal e o programa termina com erro (retorna -1)
        printf("Erro: as pontuações alteram-se jogando!\n");
        fflush(stdout);
        exit(0);
      }
      linha++;
    }while(feof(ficheiroResultados)==0); //enquanto nao atinge o fim do ficheiro
    fclose(ficheiroResultados);//fecha o documento
  }
  pontosFichResultados[linha] = pontos; //adiciona os pontos e nome do jogador atual`aos extraidos do ficheiro
  strcpy(nomeDoJogadorFichResultados[linha], nomeDoJogador);
  //organiza as pontuações dos jogadores partindo do principio que já estvam posteriormente organizadas de forma decrescente
  bubleSortExtracaoFichResultados(linha, pontosFichResultados, nomeDoJogadorFichResultados); 
  //guarda as pontuações organizadas no fichheiro
  if(guardaNovamenteNoficheiroResultados(linha, pontosFichResultados, nomeDoJogadorFichResultados)==0)
    exit(0);
}

/*
Função que recebe os vetores da pontuação dos jogadores e os seus nomes em que na ultima posição estão os dados do jogador anterior
A partir dessas matrizes organiza os dados de forma decrescente pela pontuação, supondo que os dados estavam já previamnete organizados, 
utilizando o algoritmo Buble Sort
*/
void bubleSortExtracaoFichResultados(int linha, int pontos[MAX_DIM_MATRIZ_RESULTADOS], char nome[MAX_DIM_MATRIZ_RESULTADOS][23])
{
  for (int i = linha; i > 0; --i) //percorre os indices do vetor começando pela nova pontuação
  {
    //verifica se a pontuação acima é mais alta, senão é feita uma troca
    //seguidamente se na posição acima da qual o valor mais recente foi tranferido estiver uma pontuação inferior, é feita uma outra troca e assim sucessivamente
    if (pontos[i]>pontos[i-1]) //se a nova pontuação for superior à de cada casa troca as pontuações e os nomes de ordem
    {
      //troca dos pontos entre os indices i e i -1
      int aux = pontos[i-1];
      pontos[i-1] = pontos[i];
      pontos[i] = aux;

      //troca dos nomes entre os indices i e i-1
      char strAux[23]= "";
      strcpy(strAux, nome[i-1]);
      strcpy(nome[i-1], nome[i]);
      strcpy(nome[i], strAux);
    }
    else 
      break; //caso contrário o vetor estºa já organizado e é inutil continuar a percorrer as linhas
  }
}


/*
Função que recebe as matrizes dos nomes e vetor dos pontos dos jogadores já organizadas de forma decrecente de pontos e que imprime os 
resultados no ficheiro correspondente
Retorna 1: no caso de sucesso
    0: no caso de insucesso na abertura do ficheiro 'resulttados.txt'
*/
int guardaNovamenteNoficheiroResultados(int linha, int pontosFichResultados[MAX_DIM_MATRIZ_RESULTADOS], 
    char nomeDoJogadorFichResultados[MAX_DIM_MATRIZ_RESULTADOS][23])
{
  FILE * ficheiroResultados;
  ficheiroResultados = fopen("resultados.txt", "w"); //abre o ficheiro em modo de escrita e apaga o seu conteudo

  if(ficheiroResultados == NULL) //se for impossivel abrir o ficheiro
  {
    printf("Erro: a abertura do ficheiro resultados.\n");
    return 0;
  }

  fprintf(ficheiroResultados, "Jogadores            Pontuação\n"); //imprimir o cabeçalho no ficheiro

  for (int i = 0; i <= linha; ++i)
  {
    fprintf(ficheiroResultados, "\n%-20s %07d", nomeDoJogadorFichResultados[i], pontosFichResultados[i]); //imprimir as pontuações já organizadas
  }
  printf("Pontuação guardada com sucesso!\n"); //mensagem de sucesso no terminal
  return 1;
}

