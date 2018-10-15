#include "listadeprodutos.h"

PLISTA criarLista(){
  PLISTA res = (PLISTA) malloc(sizeof(LISTADEPRODUTOS));
  res->cabeca = (PONT) malloc(sizeof(REGISTRO));
  res->cabeca->id=-1;
  res->cabeca->tipo=-1;
  res->cabeca->quantidade=0;
  res->cabeca->valorUnitario=0;
  res->cabeca->proxProd=NULL;
  return res;
}

PONT buscaAntAtual(PLISTA l, int total, PONT* ant){ //busca o item antecessor e o sucessor do registro apontado de acordo com o valor
    *ant = NULL;
    PONT atual = l->cabeca;
    while((atual!=NULL)&&((atual->valorUnitario)*(atual->quantidade)<total)){
        *ant = atual;
        if(atual->proxProd==NULL) break; //sai do la�o se estiver no �ltimo registro
        atual = atual->proxProd;
    }
    if((atual->valorUnitario)*(atual->quantidade)>=total) return atual; //aqui que ele ve se � o pr�ximo e retorna ele caso seja
    return NULL;
}



void copiador(PONT registro, int* tipo, int* qtde, int* valor){ //apenas joga os dados do registro apontado para fora da fun��o
    *tipo = registro->tipo;
    *qtde =registro->quantidade;
    *valor = registro->valorUnitario;
}

void exclusao(PLISTA l, int id){ //exclui o elemento de acordo com o ID
    PONT atual = buscarID(l, id);
    PONT ant, prox;
    int total = (atual->quantidade)*(atual->valorUnitario);
    prox = buscaAntAtual(l,total, &ant);
    if(ant==NULL && prox!=NULL)l->cabeca->proxProd = prox; //se o elemento for o primeiro
    if(prox==NULL && ant!=NULL) ant->proxProd = NULL; //se for o �ltimo
    else if(ant!=NULL && prox!=NULL) ant->proxProd = atual->proxProd; //se n�o for nem o �ltimo nem o primeiro
    free(atual);
}

void insereOrdenado(PLISTA l, PONT reg){ //utiliza o m�todo de busca para descobrir onde o registro deve ser inserido
    if(tamanho(l)==0){ //Se a lista estiver vazia...
        reg->proxProd = NULL;
        l->cabeca->proxProd = reg;
    }
    else{
        PONT ant = NULL;
        int total = (reg->quantidade)*(reg->valorUnitario);
        PONT prox = buscaAntAtual(l, total, &ant);
        if(prox==NULL){ //se o registro n�o tiver nenhum pr�ximo (ou seja, se for o �ltimo da lista)
            ant->proxProd = reg;
            reg->proxProd = NULL;
        }
        else{
            ant->proxProd = reg;
            reg->proxProd = prox;
        }
        }
}


int tamanho(PLISTA l){
  PONT atual = l->cabeca->proxProd;
  int tam = 0;
  while (atual) {
    atual = atual->proxProd;
    tam++;
  }
  return tam;
}

PONT buscarID(PLISTA l, int id){
  PONT atual = l->cabeca->proxProd;
   while (atual) {
    if (atual->id == id) return atual;
    atual = atual->proxProd;
  }
  return NULL;
}

void exibirLog(PLISTA f){
  int numElementos = tamanho(f);
  printf("Log lista [elementos: %i]\n    ", numElementos);
  PONT atual = f->cabeca->proxProd;
  while (atual){
    printf(" [%i;%i;%i;%i;$%i]", atual->id, atual->tipo, atual->quantidade, atual->valorUnitario, atual->quantidade*atual->valorUnitario);
    atual = atual->proxProd;
  }
  printf("\n\n");
}

int consultarValorUnitario(PLISTA l, int id){
  PONT atual = l->cabeca->proxProd;
   while (atual) {
    if (atual->id == id) return atual->valorUnitario;
    atual = atual->proxProd;
  }
  return 0;
}





bool inserirNovoProduto(PLISTA l, int id, int tipo, int quantidade, int valor){ //cria o registro e chama o m�todo de inserir ordenadamente
    if(buscarID(l, id)!= NULL) return false; //Se existir algum elemento com o ID do par�metro, ele returna false.
    if(id<=0 || tipo<=0 || quantidade<=0 || valor<=0) return false; //Se algum dos valores dos par�metros forem inv�lidos, retorna false;
    PONT registronovo = (PONT) malloc(sizeof(REGISTRO)); //Aloca mem�ria para o registro novo e atribui os valores dos par�metros a ele.
    registronovo->id = id;
    registronovo->tipo = tipo;
    registronovo->quantidade = quantidade;
    registronovo->valorUnitario = valor;
    insereOrdenado(l, registronovo);
  return true;
}

bool removerItensDeUmProduto(PLISTA l, int id, int quantidade){ //copia os dados do item de acordo com o ID, deleta e cria um outro com as informa��es atualizadas
    PONT atual = buscarID(l, id);
    if(atual==NULL) return false; //retorna false se n�o existir o item
    if(atual->quantidade<quantidade) return false; //retorna false se tentar remover mais itens do que j� tem no produto
    if(quantidade==atual->quantidade){ //se remover a quantidade de itens que um item possui, simplesmente delete o registro
        exclusao(l, id);
        return true;
    }
    int tipo, qtde, valor;
    copiador(atual, &tipo, &qtde, &valor);
    exclusao(l, id);
    int qtdenova = (qtde-quantidade);
    inserirNovoProduto(l, id, tipo, qtdenova, valor);
  return true;
}


bool atualizarValorDoProduto(PLISTA l, int id, int valor){ //copia os dados do item de acordo com o ID, deleta e cria um outro com as informa��es atualizadas
    PONT atual = buscarID(l, id);
    if(atual==NULL) return false; //retorna falso se n�o existir o registro
    if(valor<=0) return false; //retorna false se tentar colocar um valor menor ou igual a 0
    int tipo, qtde, valorAntigo;
    copiador(atual, &tipo, &qtde, &valorAntigo);
    exclusao(l, id);
    inserirNovoProduto(l, id, tipo, qtde, valor);
  return true;
}
