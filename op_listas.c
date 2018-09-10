/*
Ficheiro op_listas.c
Este é o ficheiro onde estão definidas todas as funções que operam sobre as listas
Estão também incluidas neste ficheiro as funções de debug, algumas utilizadas na execução do programa no modo de debug, que permitem 
dividir e analisar separadamente as operação a que as listas estão a a ser alvo, e outras de depuração :) que são utilizadas apenas no
desenvolvimento de novas funcionalidades.
*/


//----------------------------------------------------------------------INCLUDES----------------------------------------------------------------------

//incluir o header com protótipos das funções, macros, variáveis estruturadas e outros ficheiros com funções que necessitaram de ser usadas
#include "op_listas.h"

//----------------------------------------------------------------------FUNÇÕES----------------------------------------------------------------------
/*
Função que cria um vetor dinamico de apontadores do tipo apontador para BLOCO (APBLOCO), este vetor tem a dimensão do numero de colunas do jogo. 
Cada elemento do bloco é o primeiro apontador da lista dinâmica que representa as colunas da matriz, que esta função também está responsável por criar.
As listas criadas representam as colunas com bolhas de baixo para cima para que a eliminação de bolhas fosse facilitada.
Recebe como argumentos: uma variàvel do tipo DADOS e que fornecerá toda a informação acerca das dimensoes dos vetores necessária.
Retorna o apontador para a lista dinâmica e que servirá de ponto de acesso a toda a informação referente às bolhas no decorrer do programa.
*/
APBLOCO * criaListas(DADOS dados)
{
  APBLOCO * OApontador; //apontador para o vetor dinâmico
  OApontador = (APBLOCO *) calloc(dados.larguraJanela, sizeof(APBLOCO)); //alocação do vetor dinâmico

  if(OApontador == NULL) //verificar o sucesso da alocação
  {
    printf("Erro de alocação de memória.\n");
    exit(0);
  }

  for (int j = 0; j < dados.larguraJanela; ++j)//corre as colunas
  {
    APBLOCO ultimo = NULL; //apontador para o ultimo elemento adicionado à lista dinâmica

    for (int i = 0; i < dados.alturaJanela; ++i) //criar listas dinamicas com o mesmo número de elemetos que linhas do jogo 
    { 
        BOLHA bolhacriada; //cria nova bolha
        bolhacriada.cor = rand()%9 +1; //gera um número aleatório
        
        //se o jogo for inicializado no modo fixe, será necessário procuarar caminhos inicialmente, para isso marcam-se as bolhas nesse sentido
        if (dados.fixe == TRUE)
          bolhacriada.marca = SOFREU_ALTERACAO; 
        else//caso contrário a marca é inicializada como sendo nula
          bolhacriada.marca = 0;

        APBLOCO atual; //apontador para a nova bolha criada 
        atual = (APBLOCO) calloc(1, sizeof(BLOCO)); //aloca memoria para a nova bolha criada
        
        if(atual == NULL) //testa alocação
        {
          printf("Erro de alocação de memória.\n");
          exit(0);
        }

        //atribui a bolha criada ao novo bloco alocado
        atual->bolha = bolhacriada;
        atual->prox = NULL;

        if (i==0) //o apontador para o primeiro apontador de cada lista ficará guardado no vetor dinâmico
          *(OApontador+j)= atual;
        else
          ultimo->prox = atual; //criação das ligacoes da lista dinamica

        ultimo = atual;    
    }
  }

  return OApontador; //retorna o apontador para o vetor dinâmico, o unico ponto de acesso a toda a informação
}

//---------------------------------------------------------------- posição -----------------------------------------------------------------------

/*
Função que procura a bolha nas coordenadas i e j da pseudo-matriz formada pelas listas dinâmicas e retorna o endereço do bloco correspondente
Esta é uma solução que permite aceder facilmente nas outras funções desevolvidas a bolhas sabendo a sua posição relativa
Recebe como argumentos: o ponto de acesso a toda a informação, o apontador "OApontador"
                        os inteiros i e j representando as coordenadass da bolha na pseudo-matriz
                        os dados do programa através da variável estruturada DADOS
Retorna: O apontador para o bloco da bolha procurada no caso de ela existir
         NULL no caso de as coordenadas requesitadas não estarem associadas a nenhuma bolha
*/
APBLOCO apbloco_bolha_em_i_j(APBLOCO * OApontador, int i, int j, DADOS dados)
{ 

  if (i>= dados.alturaJanela || j >= dados.larguraJanela || i<0 || j<0) //se está fora dos limites da janela retorna NULL
    return NULL;

  APBLOCO step_coluna_j = *(OApontador + j); //variavel que será usada para correr a coluna j 

  int k = 0; //contador
  while(step_coluna_j != NULL) //enquanto a lista dinâmica não chega ao fim
  {
    if (k == i) //quando for atingida a linha desejada corta o ciclo
      break;
    k++;
    step_coluna_j = step_coluna_j->prox;
  }
  //retorna o endereço do bloco de memória da bolha procurada se netretanto a lista não chegou ao fim sem a ter atingido as coordenadas procuradas
  return step_coluna_j; 
}

//-------------------------------------------------------------troca de bolhas -----------------------------------------------------------------------
/*
Função que é responsável pela troca de duas bolhas
Recebe: os dados do programa
        o apontador de acesso a toda a informação das bolhas
Assim, procura as bolhas marcadas e analisa a situação em 3 casos distintos:
-> troca entre colunas que delegrará a outra funçõe
-> troca em coluna que também delegará a outra função
-> troca inválida, neste caso eliminará as marcas para troca das bolhas
*/
void troca_bolhas(DADOS dados, APBLOCO * OApontador)
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
     	if (bolhaTroca_1->bolha.marca == MARCADA_PARA_TROCA) //quando for encontrada a primeira bolha para troca sai do ciclo
     	{
     		BREAK = TRUE; //para sair dos dois ciclos usa a falg BREAK
     		break; 
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
    -> a segunda bolha a trocar não está em nenhuma das situações anteriores pelo que é uma troca inválida
  Assim, trataremos cada uma destas tres possibilidades individualmente  
  */

  //variáveis que guardarão os endereços das bolhas a cima e à direita da primeira identificada para a troca
  APBLOCO bolha_a_cima = bolhaTroca_1->prox;
  APBLOCO bolha_direita = apbloco_bolha_em_i_j(OApontador, i,j+1, dados);

  if (bolha_a_cima!= NULL && bolha_a_cima->bolha.marca == MARCADA_PARA_TROCA) //a bolha a cima foi marcada para troca
  {
    APBLOCO anterior = apbloco_bolha_em_i_j(OApontador, i-1, j, dados); //apontador para a bolha anterior necessária para a função que fará a troca
  	troca_em_coluna(OApontador, anterior, bolhaTroca_1, j);//troca na mesma coluna
  }
  else if(bolha_direita != NULL && bolha_direita->bolha.marca == MARCADA_PARA_TROCA) //a bolha à direita foi marcada para troca 
    troca_entre_colunas(OApontador, dados, bolhaTroca_1, i, j);	//troca bolhas entre colunas
  else //se a troca for inválida
    eliminar_marcas_troca(OApontador, dados, j); //elimina a marcas de troca de todas as bolhas
}

/*
Função que é responsável pela troca de bolhas na mesma coluna
Recebe: o apontador de acesso a toda a informação referente às bolhas
        o apontador para o bloco da bolha anterior à primeira que será trocada (pode ser NULL)
        o apontador para o bloco da bolha mais a baixo
        a coordenada da coluna em que se efetuará a troca
Assim, reordena as duas bolhas tratando de mudar as ligações entre os elementos das lista     
*/
void troca_em_coluna(APBLOCO * OApontador, APBLOCO anterior, APBLOCO bolhaTroca_1, int coluna)
{
  /*marca ambas as bolhas que serão trocadas com a marca SOFREU_ALTERACAO para que posteriormente se possoa avaliar 
  se a troca formou caminhos de bolhas da mesma cor*/
  bolhaTroca_1->bolha.marca = SOFREU_ALTERACAO;
  (bolhaTroca_1->prox)->bolha.marca = SOFREU_ALTERACAO;

  //se a primera bolha trocar for a primeira da lista
  if (anterior == NULL)
    *(OApontador+coluna) = bolhaTroca_1->prox; //termeos de alterar os apontadores do vetor dinâmico
  else
    anterior->prox = bolhaTroca_1->prox; 

  //reordenação das bolhas 
  APBLOCO bolhaTroca_2 = bolhaTroca_1->prox;
  bolhaTroca_1->prox = bolhaTroca_2->prox;
  bolhaTroca_2->prox = bolhaTroca_1;

}

/*
Função que é responsável pela troca de bolhas em colunas contíguas
Recebe: o apontador de acesso a toda a informação referente às bolhas
        uma variável contendo a informação dos dados do programa
        o apontador para o bloco da bolha mais à esquerda a ser trocada
        as coordenadas i, j da bolha mais à esquerda a ser trocada
Assim, reordena as duas bolhas tratando de mudar as ligações entre os elementos das listas     
*/
void troca_entre_colunas(APBLOCO * OApontador, DADOS dados, APBLOCO bolhaTroca_1,  int i, int j)
{

  APBLOCO anterior_1 = apbloco_bolha_em_i_j(OApontador, i-1,j, dados); //bolha anterior à mais à esquerda
  APBLOCO bolhaTroca_2 = apbloco_bolha_em_i_j(OApontador, i,j+1, dados); //bolha mais à direita que será trocada
  APBLOCO anterior_2 = apbloco_bolha_em_i_j(OApontador, i-1,j+1, dados); //bolha anterior à mais à direita que será trocada

  //para ter a certeza que o nosso amigo não aparece
  if (bolhaTroca_2 == NULL)
    return;

  /*marca ambas as bolhas que serão trocadas com a marca SOFREU_ALTERACAO para que posteriormente se possoa avaliar 
  se a troca formou caminhos de bolhas da mesma cor*/
  bolhaTroca_1->bolha.marca = SOFREU_ALTERACAO;
  bolhaTroca_2->bolha.marca = SOFREU_ALTERACAO;

  //variavel auxiliar para permitir a troca
  APBLOCO aux = bolhaTroca_1->prox;

  //se as bolhas forem as primeiras das listas ter-se-à de alterear os endereços guardados no vetor dinâmico
  if (anterior_1 == NULL)
  {
    *(OApontador+j+1) = bolhaTroca_1;
    *(OApontador+j) = bolhaTroca_2;
  }
  else
  {
    anterior_2->prox = bolhaTroca_1;
    anterior_1->prox = bolhaTroca_2;
  }

  //troca de endereços
  bolhaTroca_1->prox = bolhaTroca_2->prox;
  bolhaTroca_2->prox = aux;
}


/*
Função que corre todas as bolhas do jogo e anula a marca de todas as que estejam marcadas para troca
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
        as coordenadas coluna a partir da qual vale a pena procurar, pois o metodo de procura empregado anteriormente garante que a segunda
bolha a trocar estará na coluna dessa bolha ou numa colun à direita
*/
void eliminar_marcas_troca(APBLOCO * OApontador, DADOS dados, int j)
{
  for (; j < dados.larguraJanela; ++j)//percorre as linhas
  {
    APBLOCO step = *(OApontador +j);
    while(step != NULL)
    {
        
      if (step->bolha.marca == MARCADA_PARA_TROCA)
        step->bolha.marca = 0;
      
      step = step->prox;
    }
  }
}


//-------------------------------------------------------------eliminação de bolhas -----------------------------------------------------------------------

/*
Função que é responsável por marcar as bolhas que terão de ser eliminadas.
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
Retorna: TRUE se houver bolhas a eliminar
         FALSE se não houver bolhas a eliminar
Esta função corre todas as bolhas da janela e para as que estão marcadas com SOFREU_ALTERACAO verifica se existem caminhos de mais de 3 bolhas 
da mesma cor. Faz isso chamando a função recursiva "marcar_bolhas_a_eliminar_vizinhanca" que retorna o numero de bolhas formam um caminho da mesma cor.
Por forma selecionar as bolhas para eliminação marca todas as que tenham sofrido alteração com uma eliminação temporária e apenas se o número de 
bolhas no caminho formado que as contém for superior as dois, as marca para uma eliminação permanente, caso contrário a marca é anulada.
*/
int marcar_bolhas_a_eliminar(DADOS dados, APBLOCO * OApontador)
{
  int continuar_a_procurar; //variável que regulará o ciclo, será TRUE se for necessário voltar a verificar se há bolhas a eliminar
  APBLOCO bolhaAtual; //apontador para a bolha em torno da qual se averiguará se são formados caminhos da mesma cor
  int bolhas_cadeia = 0; //variável que guardará o número de bolhas numa determinada cadeia
  //variável que serivirá para verificar se no correr desta função haverá bolhas que precisarão se ser posteriormente eliminadas
  int vao_ser_eliminadas_bolhas = FALSE; 
  
  do //ciclo que corre enqunto houver bolhas que tenham sofrido alteração para as quais ainda não foi verificado se forma caminhos
  {
    continuar_a_procurar = FALSE;

    for (int j = 0; j < dados.larguraJanela; ++j)//percorre as linhas
    { 
      int i = 0; //coordenada na coluna
      bolhaAtual = *(OApontador + j);
      while(bolhaAtual != NULL)
      {
        if (bolhaAtual == NULL) //se não existirem mais bolhas numa coluna, passa à seguinte
          break;

        if (bolhaAtual->bolha.marca == SOFREU_ALTERACAO) //se a bolha sofreu alteração verifica se forma caminhos
        {
          bolhaAtual->bolha.marca = MARCADA_PARA_ELIMINACAO_TEMP; //marca a bolha para eliminação temporária
          continuar_a_procurar = TRUE; //procura pelo menos mais uma vez
          //verifica as cadeias à volta dessa bolha e verifica se forma uma cadeia com mais de 2 elementos
          bolhas_cadeia = marcar_bolhas_a_eliminar_vizinhanca(dados, OApontador, i, j) + 1;
          if(bolhas_cadeia > 2)//se tiver mais de dois elementos
          {
            //as bolhas que estavam marcadas para eliminação temporária passam agora a eliminação permanente
            marca_bolhas_candidatas_eliminacao_permanente(dados, OApontador); 
            vao_ser_eliminadas_bolhas = TRUE; //neste caso já sabemos que vão ser eliminadas bolhas
          }
          //caso contrário a marca das bolhas que estavam marcadas para eliminação temporária é anulada
          else
            marca_bolhas_candidatas_a_0(dados, OApontador); 
        }
        bolhaAtual = bolhaAtual->prox;
        i++; 
      }
    }
  }while(continuar_a_procurar == TRUE); //enqunto não forem averiguadas todas as bolhas que sofreram alteração

  return vao_ser_eliminadas_bolhas;
}


/*
Função recursiva que é responsável pela marcação temporária de todas as bolhas que formem caminhos de uma dada cor
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
        a coordenada da bolha que numa determinada instância servirá de centro em trono do qual as bolhas serão avaliadas
Retorna: O numero de bolhas que encontrou da mesma cor do que a bolha central dada e que foram encontradas em todas as vezes que se invocou a si própria
pelo que no final da execução da primeira vez que foi invocada retornará o núremo de bolhas que formam uma cadeia de uma determinada cor.

Esta função recebe na sua primeira invocação as coordenadas de uma bolha apartir do qual ela terá de avaliar se existe um caminho que a contenha. Para 
isso verifica se as bolhas contíguas a ela são da mesma cor e não foram já marcadas para eliminação (caso contrário gerar-se-ia um ciclo infinito). Para
cada bolha que encontre da mesma cor chamar-se-á a si própria pr forma a avaliarv a vizinhamnça dessa bolha e a assim sucessivamente. Até que encontra
uma bolha que não forma já mais caminhos com a sua vizinhança pelo que retorna 0. Isto fará com que a última instância a ser invocada desta função acabe, 
e que as intruções da função que a invocou prosigam (procurando outra bolha ou retornando o valor de bolhas que encontrou, que , neste caso, será maior
que 1) até chegar à primeira função invocada e ser retornado para a função "marcar_bolhas_a_eliminar" o numero de bolhas a formar o caminho encontrado.
*/
int marcar_bolhas_a_eliminar_vizinhanca(DADOS dados, APBLOCO * OApontador, int i, int j)
{

  int bolhas_encontradas = 0; //variável que guardará as bolhas em caminho encontradas por cada instância desta função

  APBLOCO bolha_a_testar; //apontador para a bolha a testar
  APBLOCO bolha_central = apbloco_bolha_em_i_j(OApontador, i, j, dados); //bolha em torno da qual se procurarão bolhas da mesma cor

  //---------------------------------------------------------------------testar bolha em cima ---------------------------------------------
  bolha_a_testar = apbloco_bolha_em_i_j(OApontador, i+1, j, dados);
  if (bolha_a_testar != NULL)
  {
    if (bolha_a_testar->bolha.marca != MARCADA_PARA_ELIMINACAO_TEMP && bolha_a_testar->bolha.marca != MARCADA_PARA_ELIMINACAO && 
        bolha_a_testar->bolha.cor == bolha_central->bolha.cor) //se a bolha for da mesma cor e não tiver sido já marcada para eliminação temporária ou permanete então marca-a
    {    
      bolha_a_testar->bolha.marca = MARCADA_PARA_ELIMINACAO_TEMP; //marca para eliminação temporária 
      bolhas_encontradas++; //encontrou uma bolha da mesma cor pelo que bolhas_encontradas aumentará em 1 
      //irá correr a mesma função em trono da bolha que encontro recentemente para que se possa continuar o teste e verificar a existência de caminhos
      bolhas_encontradas += marcar_bolhas_a_eliminar_vizinhanca(dados, OApontador, i+1, j);  
    }
  }

  //---------------------------------------------------------------------testar bolha á direita ---------------------------------------------
  bolha_a_testar = apbloco_bolha_em_i_j(OApontador, i, j+1, dados);
  if (bolha_a_testar != NULL)
  {
    if (bolha_a_testar->bolha.marca != MARCADA_PARA_ELIMINACAO_TEMP && bolha_a_testar->bolha.marca != MARCADA_PARA_ELIMINACAO && bolha_a_testar->bolha.cor == bolha_central->bolha.cor)
    { 
      bolha_a_testar->bolha.marca = MARCADA_PARA_ELIMINACAO_TEMP;
      bolhas_encontradas++;
      bolhas_encontradas += marcar_bolhas_a_eliminar_vizinhanca(dados, OApontador, i, j+1);
    }
  }

  //---------------------------------------------------------------------testar bolha em baixo ---------------------------------------------
  bolha_a_testar = apbloco_bolha_em_i_j(OApontador, i -1, j, dados);
  if (bolha_a_testar != NULL)
  {
    if (bolha_a_testar->bolha.marca != MARCADA_PARA_ELIMINACAO_TEMP && bolha_a_testar->bolha.marca != MARCADA_PARA_ELIMINACAO && bolha_a_testar->bolha.cor == bolha_central->bolha.cor)
    {
      bolha_a_testar->bolha.marca = MARCADA_PARA_ELIMINACAO_TEMP;
      bolhas_encontradas++;
      bolhas_encontradas += marcar_bolhas_a_eliminar_vizinhanca(dados, OApontador, i-1, j);
    }
  }

  //---------------------------------------------------------------------testar bolha á esquerda ---------------------------------------------
  bolha_a_testar = apbloco_bolha_em_i_j(OApontador, i, j-1, dados);
  if (bolha_a_testar != NULL)
  {
    if (bolha_a_testar->bolha.marca != MARCADA_PARA_ELIMINACAO_TEMP && bolha_a_testar->bolha.marca != MARCADA_PARA_ELIMINACAO && bolha_a_testar->bolha.cor == bolha_central->bolha.cor)
    {
      bolha_a_testar->bolha.marca = MARCADA_PARA_ELIMINACAO_TEMP;
      bolhas_encontradas++;
      bolhas_encontradas += marcar_bolhas_a_eliminar_vizinhanca(dados, OApontador, i, j-1);
    }
  }

  return bolhas_encontradas;
}

/*
Função que corre todas as bolhas do jogo e marca para eliminação permanente todas as que estejam marcadas para eliminação temporária
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
*/
void marca_bolhas_candidatas_eliminacao_permanente(DADOS dados, APBLOCO * OApontador)
{
  for (int j = 0; j < dados.larguraJanela; ++j)//verifica todas as colunas
  {
    APBLOCO bolhaAtual = *(OApontador +j);
    while(bolhaAtual != NULL)
    {  
      if (bolhaAtual == NULL)
        break;
      if(bolhaAtual->bolha.marca == MARCADA_PARA_ELIMINACAO_TEMP)//se encontrar bolha marcada para eliminação temporária 
        bolhaAtual->bolha.marca = MARCADA_PARA_ELIMINACAO;//marca para eliminação permanente

      bolhaAtual = bolhaAtual->prox;
    }
  }
}

/*
Função que corre todas as bolhas do jogo e anula a marca de todas as bolhas que estejam marcadas para eliminação temporária
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
*/
void marca_bolhas_candidatas_a_0(DADOS dados, APBLOCO * OApontador)
{
  for (int j = 0; j < dados.larguraJanela; ++j)//percorre as linhas
  {
    APBLOCO bolhaAtual = *(OApontador +j);
    while(bolhaAtual != NULL)
    {
      if (bolhaAtual == NULL)
        break;
      if(bolhaAtual->bolha.marca == MARCADA_PARA_ELIMINACAO_TEMP)//se encontrar bolha marcada para eliminação temporária
        bolhaAtual->bolha.marca = 0;//anula a marca

      bolhaAtual = bolhaAtual->prox;
    }
  }
}

/*
Função que elimina todas as bolhas que estejam marcadas para eliminação permante e que marca todas as bolhas acima de uma bolha eliminada como tendo 
sofrido alteração para que possa, posteriormente, ser verificado se essa eliminação formou novos caminhos.
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
Retorna: o numero de bolhas eliminadas e que será usado na contagem dos pontos do jogador
*/
int elimina_bolhas_marcadas(DADOS dados, APBLOCO * OApontador)
{
  int bolhas_rebentadas_total = 0; //variável que guardará o numero de bolhas rebentadas 

  for (int j = 0; j < dados.larguraJanela; ++j)//verifica todas as colunas
  {
    APBLOCO step = NULL, anterior = NULL; //variáveis para correr todas as bolhas
    APBLOCO apaga; //variável para guardar a bolha a eliminar
    int eliminou = FALSE; //variável que verifica se foram eliminadas bolhas numa coluna em particular
  
    step = *(OApontador +j);
    while(step != NULL)//percorre as colunas
    {
      if (step->bolha.marca == MARCADA_PARA_ELIMINACAO)
      { 
        //se a bolha é marcada para eliminação é feito free do bloco e reorganização da lista
        apaga = step;
        step = step->prox;
        free(apaga);
        bolhas_rebentadas_total++;
        eliminou = TRUE; //peo menos uma bolha foi eliminada na coluna j

        if (anterior == NULL) //se a bolha eliminada é a primeira da lista, é necessário alterar os endereços do vetor dinâmico
          *(OApontador +j) = step;
        else
          anterior->prox = step;
      }
      /*todas as bolhas a cima de uma bolha que tenha sido eliminada numa coluna têm que ser marcadas com tendo sofrido alteração. Assim é possível 
      verificar novamente se a eliminação de algumas bolhas formou novos caminhos*/
      else 
      {
        if (eliminou == TRUE) //se já foi eliminada uma bolha nesta coluna
          step->bolha.marca = SOFREU_ALTERACAO;//marcada como tendo sofrido ateração
        anterior = step;
        step = step->prox;
      }
    }
  }

  return bolhas_rebentadas_total;
}

/*
Função que no modo fixe garante que inicialmente não aparecem bolhas a formar já caminhos. Para isso, como no modo fixe todas as bolhas são marcadas
como inicialmente terem sofrido alteração, as funções de marcação e eliminação descreitas anteriormente podem ser utilizadas.
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
*/
void remove_bolhas_iniciais_em_caminhos(DADOS dados, APBLOCO * OApontador)
{
  while(TRUE) //até haver bolhas a eliminar (até ainda haver caminhos)
  {
    if(marcar_bolhas_a_eliminar(dados, OApontador)==FALSE) //marca bolhas em caminhos se já não existirem caminhos sai do ciclo
      break;
    elimina_bolhas_marcadas(dados, OApontador); //elimina as bolhas
    adiciona_bolhas(OApontador, dados, FALSE); //como está no modo fixe adiciona bolhas, mas sem animação
  }  
}


//------------------------------------------------------------- adição de bolhas -----------------------------------------------------------------------

/*
Função que no modo fixe adiciona bolhas para substituir aquelas que foram eliminadas.
Recebe: o apontador de acesso a toda a informação das bolhas
        os dados do jogo
        o inteiro com_animação que pode tomar os valores TRUE ou FALSE e que indica se a adição de bolhas será feita para haver posterior animação da 
queda ou não.
Assim, a função adiciona bolhas marcando-as de dois modos distintos: marcadas como tendo sofrido alteração para que outra função depois avalie se há 
novos caminhos formados ou de acordo com a sua posições relativas na sua coluna imaginando-as numa nova matriz acima da janela, mecanismo usado para 
facilitar a animação da queda.        
*/
void adiciona_bolhas(APBLOCO * OApontador, DADOS dados, int com_animacao)
{

  for (int j = 0; j < dados.larguraJanela; ++j) //verifica todas as colunas
  {
    APBLOCO step = *(OApontador + j), ultimo = NULL; //variáveis que varrerão as colinas
    int i = 0; //variável que controlará o número de bolhas da coluna e regulará a adição de novas bolhas
    while(step != NULL)//qundo step for NULL o ciclo acabará e anterior guardará o endereço da última bolha da coluna
    {
      ultimo = step;
      step = step->prox;
      i++;
    }

    for (int k = 0; k < dados.alturaJanela -i; ++k) //cria o número de bolhas necessário para a coluna ficar completa
    {
      BOLHA bolhacriada; //cria nova bolha
      bolhacriada.cor = rand()%9 +1; //aleatório
      
      /*Para adicionar bolhas sem animação basta marca-las com tendo sofrido alteração para posteriormente ser avaliado se a formação das bolhas formou
      nocos caminhos, este modo é utilizado apenas ao procurar no inicio do jogo bolhas que façam já caminhos.
      Se estivermos no modo fixe a forma de fazer com que as bolhas adicionadas caiam é imaginá-las numa outra pseudo-matriz acima da janela de jogo 
      em que as bolhas têm que estar o mais abaixo possível na sua coluna, assim, uma formas de represenatr as suas posições relativas nessa matriz 
      é adicionar ao número ANIMACAO_DESCER a sua posição contada relativamente ao fundo dessa nova matriz imaginária para que posteriormete de possa 
      através dessas posições animar a descida.*/
      if (com_animacao == FALSE) 
         bolhacriada.marca = SOFREU_ALTERACAO;
      else
        bolhacriada.marca = ANIMACAO_DESCER + k;
  
      APBLOCO atual; //apontador para a nova bolha criada 
      atual = (APBLOCO) calloc(1, sizeof(BLOCO)); //aloca memoria para a nova bolha criada
      
      if(atual == NULL) //testa alocação
      {
        printf("Erro de alocação de memória.\n");
        exit(0);
      }
  
      atual->bolha = bolhacriada; //pões bolha no espaço útil do bloco
      atual->prox = NULL;
  
      if (ultimo == NULL) //o apontador para o primeiro apontador de cada lista ficará guardado no vetor dinâmico
        *(OApontador+j)= atual;
      else
        ultimo->prox = atual; //criação das ligacoes da lista dinamica
  
      ultimo = atual;
    }
  }
}


//------------------------------------------------------------- controlo -----------------------------------------------------------------------
/*
Função que no modo normal, onde não caem novas bolhas, verifica se há mais hipóteses de jogada.
Recebe: o apontador de acesso a toda a informação das bolhas
        os dados do jogo
Retorna: TRUE se for possível continuar ou FALSE se não houver mais possibilidades de jogada
Esta função faz conjuntos de colunas da matriz imaginária tal que essas colunas sejam contíguas umas às outras e que o conjuntos sejam separados por 
colunas em branco (que não são incluidas neles). Assim, é verificada cada porção de bolhas do conjunto entre colunas em branco, pois sabemos que 
é impossível trocar bolhas entre duas colunas se entre elas existir uma completamente vazia.
Desta forma a função varre todas as bolhas em todos os conjuntos e guarda o numero de bolhas de cada cor numa matriz que posteriormente será avaliada
por uma outra função. Se para alguma cor houver mais de tres bolhas no mesmo conjunto o jogo pode continuar caso contrário esta função retorna FALSE.
*/
int verifica_estado_do_jogo(APBLOCO * OApontador, DADOS dados)
{
  int extremo_inferior = 0; //variável que guardará o extremo inferior de cada conjunto de bolhas
  int extremo_superior = 0; //variável que guardará o extremo superior de cada conjunto de bolhas

  int numero_de_cores[9][2];//matriz para guardar o número de bolhas de cada cor
  
  int inicio = TRUE; //flag que indica se é a primeira vez que o loop está a ser feito
  
  int j = 0;
  APBLOCO step_colunas; //variável para correr todas as bolhas da coluna

  while(TRUE)
  {
    if (inicio == FALSE) //se já tiver avaliado um conjunto é necessário encontrar o pr´ximo limite inferior
    {
      extremo_superior++;
      //vai correndo o extremo superior a partir da posição seguinte ao extremo superior da itersção anterior
      for ( ;extremo_superior < dados.larguraJanela; extremo_superior++) 
      {
        if (*(OApontador + extremo_superior) != NULL) //se não for uma coluan vazia, é esse o nosso novo etremo inferior
        {
          extremo_inferior= extremo_superior;
          break;
        }    
      }
      //se o extremo superior é igual ao limite da janela é o ciclo acaba
      if (extremo_superior == dados.larguraJanela)
        break;
    }

    inicio = FALSE;

    memset(numero_de_cores, 0, sizeof(numero_de_cores[0][0]) * 9 * 2); //poe a matriz a 0
    j = extremo_inferior;
    //saltita de coluna em coluna até encontrer uma coluna vazia ou acabarem as colunas a verificar
    for (step_colunas = *(OApontador + j); step_colunas != NULL && j< dados.larguraJanela; j++, step_colunas = *(OApontador +j));
    extremo_superior = j-1; //atribui essa posição ao extremo superior

    //não há mai colunas a procurar
    if (extremo_superior >= dados.larguraJanela || extremo_inferior >= dados.larguraJanela) 
      break;
    //se a primeira coluna estiver vazia isto acontece, pelo que queremos procurar na coluna seguinte
    else if (extremo_superior < extremo_inferior)
      continue;
    //conseguiu arranjar um conjunto válido
    else
    {
      for (; extremo_inferior <= extremo_superior; extremo_inferior++) //varre todas as bolhas do conjunto 
      {
        APBLOCO step;
        step = *(OApontador + extremo_inferior);
  
        for (; step != NULL; step = step->prox)
        {
          numero_de_cores[step->bolha.cor-1][1]++; //vai atualizando a matriz
        }
      }
    }

    if(avalia_matriz(numero_de_cores) == 1) //dá a matriz a uma outra função para avaliar se naquele conjunto é possivel formar caminhos
      return TRUE; //se sim retorna TRUE e não é necessário avaliar os restantes conjuntos pois é possível já em um   
  }

  return FALSE;
}

/*
Função que avalia a matriz da quantidade de bolhas de cada cor e se alguma for maior que dois retorna TRUE
Recebe: a matriz com o número debolhas de cada cor
Retorna: TRUE se houver mais de duas bolhas de alguma cor
         FALSE no caso contrário
*/
int avalia_matriz(int numero_de_cores[9][2])
{
  for (int i = 0; i < 9; ++i) //coore toda a coluna 
  {
    if (numero_de_cores[i][1] > 2)//se algum é maior de dois
      return TRUE;
  }

  return FALSE;
}

/*
Função que liberta a memória ocupada pelas bolhas e plo vetor dinâmico
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
Retorna: NULL que será atribuido a OApontador (sgnifica que já não há bolhas acessíveis)
*/
APBLOCO * byebye_listas(DADOS dados, APBLOCO * OApontador)
{
  APBLOCO step, anterior;
  for (int j = 0; j < dados.larguraJanela ; ++j) //verifica todas as colunas
  {
    anterior = *(OApontador + j);
    if (anterior == NULL) //a coluna não tem bolhas logo passa à coluna seguinte
     continue;

    for (step = anterior->prox; step != NULL; step = step->prox) //começa em baixo e vai libertando a memória das bolhas ao longo da coluna
    {
      free(anterior);
      anterior = step;
    }
    free(anterior); //desaloca a última bolha

  }

  free(OApontador); //desaloca o vetor dinâmico
  return NULL;
}

//------------------------------------------------protótipos das funções de debug sobre operções de listas ----------------------------------------

/*
Função que entre operações de listas para a execução do programa até que o jogador carregue em ENTER
Recebe: string com o motivo da paragem
*/
void DEBUG(char str[])
{
  printf("Enter para %s", str);
  getchar();
}

/*
Função que imprime todas as bolhas na matriz, evidenciando toda a sua informação, endereço, cor e marca
Recebe: os dados do jogo
        o apontador de acesso a toda a informação das bolhas
*/
void estado(DADOS dados, APBLOCO * OApontador)
{
  printf("----------------------------------------------\n");
  for (int j = 0; j < dados.larguraJanela; ++j)//percorre todas as colunas
  {
    int i = 0;
    APBLOCO bolhaAtual = *(OApontador +j);
    while(bolhaAtual != NULL)
    { 
      printf("%p", (void *) bolhaAtual); fflush(stdout);
      printf("(%d,%d) c:%d m:%d | ", i, j, bolhaAtual->bolha.cor, bolhaAtual->bolha.marca); fflush(stdout);   
      bolhaAtual = bolhaAtual->prox;
      i++;
    }
    printf("\n");
    fflush(stdout);
  }
  printf("----------------------------------------------\n");
}
