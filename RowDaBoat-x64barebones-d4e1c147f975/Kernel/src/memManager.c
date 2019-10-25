#include <memManager.h>
#include <memManager.h>
#include "../include/memManager.h"
#include <buddyManager.h>

/*
  Meotodos genericos para decidir que tipo de mem manager utilizaremos.
*/

static int memManagerUsed = BUDDY_MANAGER;

void * mAlloc(size_t size){
  if(memManagerUsed == FIRST_FIT_MANAGER){
    return memMalloc(size);
  }
  else{
    return buddyMalloc(size);
  }

}


int mFree(void * ptr){
  if(memManagerUsed == FIRST_FIT_MANAGER){
    return memFree(ptr);
  }
  else{
    return buddyFree(ptr);
  }
}

void initializeMemManager(void * startDir, size_t totalSize){
     if(memManagerUsed == FIRST_FIT_MANAGER){
      initializeMemManagerList(startDir,totalSize);
  }
  else{
     initializeBuddyMemory(startDir,totalSize);
  }
}


//Todo de first fit

/*
  Estructura del tipo Nodo
*/
struct t_node {
  // Proximo nodo
  struct t_node * next;
  // Puntero a memoria libre
  void * memPtr;
  // Tamaño de esa memoria libre
  size_t size;
  // Estado del bloque
  enum STATE state;
};
/*
  Estructura del tipo Lista
*/
struct t_list {
  // Primer elemento de la lista
  struct t_node * head;
  // Puntero al comienzo de la memoria
  void * startDir;
  // Tamaño total de la memoria
  size_t totalSize;
};

// Redefinicion del puntero al tipo lista
typedef struct t_list * List;

Node firstFit(Node node, size_t size);
int freeMemory(Node actual, Node previous, void * ptr);

static List memBlocks;

/*
  Algoritmo de FIRST FIT, es RECURSIVO.
  Busca el primer bloque de la memoria que este libre y tenga espacio suficiente.
  Si le sobra espacio a un bloque, y puede alocar otro nodo, lo hace.
  Parametros:
    Node node --> Nodo actual
    size_t size --> Tamaño de memoria para alocar
  Devuelve:
    Node --> Nodo que tiene el tamaño requerido para alocar
*/
Node firstFit(Node node, size_t size){
  // En el caso de que no haya espacio o lleguemos al final de la lista
  if (node == NULL || node->size == 0){
    return NULL;
  }

  if (node->state == FREE){
    // En el caso de que el tamaño sea el justo, solo marca al bloque como ocupado
    if (node->size == size){
      // Marcamos al nodo como que NO esta libre
      node->state = NOT_FREE;
      return node;
    }
    else if (node->size > size){
      // Calcula la cantidad de memoria que sobraria despues de otorgar lo que se pide
      int spaceLeft = node->size - size;

      // En el caso de que haya sobre la cantidad como para armar un nodo mas, lo aloca
      if (spaceLeft > sizeof(struct t_node)){
        // Calcula el puntero del nuevo nodo
        Node aux = (void *)((char *)node->memPtr + size);
        // Para no perder la referencia al siguiente
        aux->next = node->next;
        // Marca al nodo como libre
        aux->state = FREE;
        // Calcula el puntero a la memoria, sin contar el tamaño de este nodo
        aux->memPtr = (void *)((char *)aux + sizeof(struct t_node));
        // Calcula el tamaño de la memoria porque se uso parte para el nodo
        aux->size = spaceLeft - sizeof(struct t_node);

        // Le cambia el tamaño al bloque porque se corto la cantidad de memoria que tiene
        node->size = size;
        node->next = aux;
      }

      // Lo marcamos como que NO esta libre
      node->state = NOT_FREE;

      return node;
    }
  }

  return firstFit(node->next, size);
}

/*
  Metodo para alocar memoria.
  Usa el algoritmo FIRST FIT, con una lista simplemente encadenada.
  Parametros:
    size_t size --> Tamaño de memoria para alocar
  Devuelve:
    void * --> Puntero a la direccion de memoria para empezar a alocar
*/
void * memMalloc(size_t size){
  Node node = firstFit(memBlocks->head, size);
  if (node == NULL){
    return NULL;
  }
  return node->memPtr;
}

/*
  Metodo para liberar la memoria de forma RECURSIVA.
  Hay 4 posibles escenarios en la liberacion:
  Caso 1:
        Nodo Anterior --> Ocupado/null
        Nodo Siguiente --> Ocupado/null
  --> Libero al encontrado
  Caso 2:
        Nodo Anterior --> Libre
        Nodo Siguiente --> Ocupado/null
  --> Mergeo el actual al anterior
  Caso 3:
        Nodo Anterior --> Libre
        Nodo Siguiente --> Libre
  --> Mergeo el actual y el siguiente al anterior
  Caso 4:
        Nodo Anterior --> Ocupado/null
        Nodo Siguiente --> Libre
  --> Mergeo el siguiente al actual
  Parametros:
    Node actual --> Nodo actual que se esta navegando
    Node previous --> Nodo anterior por el que se navego
    void * ptr --> Puntero a liberar
  Devuelve:
    int --> 1 si fue exitoso, 0 si no lo fue
*/
int freeMemory(Node actual, Node previous, void * ptr){
  // En el caso de que lleguemos al final o que nos pasen un puntero nulo
  if (actual == NULL || ptr == NULL){
    return 0;
  }

  // En este caso encontramos al puntero que buscamos
  if (actual->memPtr == ptr){
    // Caso de ANTERIOR --> OCUPADO o NULL y SIGUIENTE --> OCUPADO o NULL
    if ((previous == NULL || (previous != NULL && previous->state == NOT_FREE)) && ((actual->next != NULL && actual->next->state == NOT_FREE) || actual->next == NULL)){
      actual->state = FREE;
      return 1;
    }
    // Caso de ANTERIOR --> LIBRE y SIGUIENTE --> OCUPADO o NULL
    else if (previous != NULL && previous->state == FREE && ((actual->next != NULL && actual->next->state == NOT_FREE) || actual->next == NULL)){
      previous->size = previous->size + actual->size + sizeof(struct t_node);
      previous->next = actual->next;
      return 1;
    }
    // Caso de ANTERIOR --> LIBRE y SIGUIENTE --> LIBRE
    else if (previous != NULL && actual->next != NULL && previous->state == FREE && actual->next->state == FREE){
      previous->size = previous->size + actual->size + actual->next->size + (2 * sizeof(struct t_node));
      previous->next = actual->next->next;
      return 1;
    }
    // Caso de ANTERIOR --> OCUPADO o NULL y SIGUIENTE --> LIBRE
    //else if ((previous == NULL || (previous != NULL && actual->next != NULL)) && previous->state == NOT_FREE && actual->next->state == FREE){
    else if ((previous == NULL || (previous->state == NOT_FREE)) && actual->next!= NULL && actual->next->state == FREE){
      actual->size = actual->size + actual->next->size + sizeof(struct t_node);
      actual->next = actual->next->next;
      actual->state = FREE;
      return 1;
    } else {
      return 0;
    }
  } else {
    return freeMemory(actual->next, actual, ptr);
  }
}

/*
  Metodo para liberar la memoria.
  Parametros:
    void * ptr --> Puntero a liberar
  Devuelve:
    int --> 1 si fue exitoso, 0 si no lo fue
*/
int memFree(void * ptr){
  int result = freeMemory(memBlocks->head, NULL, ptr);
  return result;
}

/*
  Metodo para inicializar la lista.
  Calcula el tamaño del primer nodo y de la lista, teniendo en cuenta el espacio que ocupa el
  puntero a la lista y al primer nodo.
  Parametros:
    void * startDir --> Direccion donde comienza la memoria
    size_t totalSize --> Tamaño de la memoria
*/
void initializeMemManagerList(void * startDir, size_t totalSize){
  memBlocks = startDir;

  memBlocks->totalSize = totalSize;
  memBlocks->startDir = startDir;
  memBlocks->head = (void * ) ((char *)startDir + sizeof(struct t_list));

  memBlocks->head->next = NULL;
  memBlocks->head->memPtr = (void * ) ((char *)memBlocks->head + sizeof(struct t_node));
  memBlocks->head->size = totalSize - sizeof(struct t_list) - sizeof(struct t_node);
  memBlocks->head->state = FREE;
}
