#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Representação de Elementos em Lista encadeada Dupla
typedef struct sElemento{
    struct sElemento *next;
    struct sElemento *prev;
    char dado; 
} Elemento;


//Representação de Lista encadeada Dupla, mesma coisa que a simples
typedef struct {
    Elemento *head;
    Elemento *tail;
    int size;
} Lista;


//Prototipação
Lista* criaLista();
Elemento* criaElemento(char);
int insereElementoNaLista(Lista*, Elemento*, char);
int insereNaPilha(Lista*, char);
int removeElementoDaLista(Lista*, Elemento*);
void removeDaPilha(Lista*);
void percorreLista(Lista*);
void percorreListaNoOutroSentido(Lista*);
void limpaLista(Lista*);
Elemento* pesquisaNaLista(Lista*, int);
void validaExpressao(Lista*, char*);

//função cria lista e desloca memória para ela
Lista* criaLista(){
    Lista* lista;
    lista = (Lista*) malloc(sizeof(Lista));
    if (lista == NULL)
        return NULL;
    else {
        lista->head = NULL;
        lista->tail = NULL;
        lista->size = 0;
    }
    
    return lista;
}

// função cria elemento e desloca memória para ele
Elemento* criaElemento(char dado){
    Elemento* nodo;
    nodo = (Elemento*) malloc(sizeof(Elemento));
    if (nodo == NULL)
        return NULL;
    else {
        nodo->next = NULL;
        nodo->prev = NULL;
        nodo->dado = dado;
    }  
    return nodo;
}

//função insere elementos na lista dupla
int insereElementoNaLista(Lista* lista, Elemento* pivo, char dado){  
  
  Elemento* novo = criaElemento(dado);
  
  if(pivo == NULL && lista->size != 0){
    
    return -1; //só aceita pivo nulo na inserção do primeiro elemento       
  }
  
  if(lista->size == 0){ //verifica se a lista está vazia
    
    lista->head = novo;
    lista->tail = novo;
    
    
  }else{

    novo->next = pivo->next; //se não tiver, só add elementos na sequência
    novo->prev = pivo;

    if(pivo->next == NULL){
      lista-> tail = novo;
    }else{
      pivo->next->prev = novo;
    }

    pivo->next = novo;
    
  }
  
  lista->size++;
  return 0;
  
}

//insere elementos na PILHA
int insereNaPilha(Lista* fila, char x){   
  insereElementoNaLista(fila, fila->tail, x); 
  return x; 
}

//função de remover elementos da lista
int removeElementoDaLista(Lista* lista, Elemento* aux){
  int dado;
  if(aux!= NULL && lista->size != 0){
    if(aux == lista->head){
      lista->head = aux->next;
      
      if(lista->head == NULL){
        lista->tail = NULL;
        
      }else{
        aux->next->prev = NULL;
      }
      
    }else{
      aux->prev->next = aux->next;
      
      if(aux->next == NULL){
        lista->tail = aux->prev;
        
      }else{
        aux->next->prev = aux->prev;
      }
    }
    dado = aux->dado;
    free(aux);
    lista->size++;
    return dado;         
  } else {
     return printf("Auxiliar é nulo ou número não existe na lista");  //caso não consiga remover
  } 
}


void removeDaPilha(Lista* lista){
  Elemento* aux;

  if(lista->size == 0){
    printf("ERRO DE UNDERFLOW");
    return ;
  }else{
    
    aux = lista->tail;
    
    if(lista->size == 1){
      
      lista->head = NULL;
      lista->tail = NULL;
      
    }else{
      
      aux->prev->next = NULL;
      lista->tail = aux->prev;
    }
    
    lista->size--;
    free(aux);
  }
  
}




//função limpa a lista e libera memória
void limpaLista(Lista* lista){
    while(lista->head != NULL){
        //removeElementoDaLista(lista, lista->head); //função remove não aceita null
      removeDaPilha(lista);
    }
    
    //free(lista);
}


//percorre a lista no sentido head->tail
void percorreLista(Lista* lista){
    Elemento* aux;
    aux = lista->head;
    
    while(aux != NULL){
        printf("%i, ", aux->dado);
        aux = aux->next;
    }
}

//percorre a lista no sentido tail->head
void percorreListaNoOutroSentido(Lista* lista){
    Elemento* aux;
    aux = lista->tail;
    
    while(aux != NULL){
        printf(" %i, ", aux->dado);
        aux = aux->prev;
    }
}

//percorre a lista para encontrar um elemento especifico
Elemento* pesquisaNaLista(Lista* lista, int dado){
    Elemento* aux;
    aux = lista->head;
    
    while(aux != NULL){
        if (aux->dado == dado){
            return aux;
        }
        aux = aux->next;
    }
    return NULL;
}

void validaExpressao(Lista* pilha, char* expressao){
  int i;
  //printf("EXPRESSAO DIGITADA: %s",expressao);

  for(i = 0;i < strlen(expressao);i++){

		if((expressao[i] == '(') || (expressao[i] == '[') || (expressao[i] == '{')){
			
			//insereElementoNaPilha(pilha, pilha->tail, expressao[i]);
      insereNaPilha(pilha, expressao[i]);
      
		}else if ((expressao[i] == ')') || (expressao[i] == ']') || (expressao[i] == '}')){
			if(pilha->size != 0){  // verifica se a pilha esta vazia
				
				if(expressao[i] == ')' && pilha->tail->dado == '('){
          removeDaPilha(pilha);
        }else if(expressao[i] == ']' && pilha->tail->dado == '['){
          removeDaPilha(pilha);
        }else if(expressao[i] == '}' && pilha->tail->dado == '{'){
          removeDaPilha(pilha);
        }else{
          printf("EXPRESSAO INVALIDA, ERRO DE ESCOPO\n");
          return;
        }
				
			}else{
				printf("EXPRESSAO INVALIDA, ERRO DE UNDERFLOW!\n");
        return;
	
			}
		}
	}

  if(pilha->size == 0){
    printf("EXPRESSAO VALIDA\n");
  }else{
    printf("EXPRESSAO INVALIDA, SOBROU ELEMENTOS\n");
  }
	
}



int main(void) {

  Lista* pilha = criaLista();
  char expressao[50] = "{[A + B] - [(C - D)]";

  /*insereNaPilha(pilha, "(A + B} )"); //invalido
  insereNaPilha(pilha, "{[A + B] - [(C - D)]"); //invalido
  insereNaPilha(pilha, "(A + B)-{C + D}-[F+ G]"); //valido
  insereNaPilha(pilha, "((H) * {([J + K])}) "); //valido
  insereNaPilha(pilha, "(((A))))");*/ //invalido

  
  validaExpressao(pilha,"(A + B} )");
  printf("\n");
  limpaLista(pilha);
  
  validaExpressao(pilha, "{[A + B] - [(C - D)]");
  printf("\n");
  limpaLista(pilha);
  
  validaExpressao(pilha, "(A + B)-{C + D}-[F+ G]");
  printf("\n");
  limpaLista(pilha);
  
  validaExpressao(pilha, "((H) * {([J + K])}) ");
  printf("\n");
  limpaLista(pilha);
  
  validaExpressao(pilha, "(((A))))");
  printf("\n");
  limpaLista(pilha);
  
 
  return 0;
}