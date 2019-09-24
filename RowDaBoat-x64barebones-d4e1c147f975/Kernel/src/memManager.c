#include <memManager.h>

/*
  Estructura del tipo Nodo
*/
struct t_node {
  // Proximo nodo
  struct t_node * next;
  // Puntero a memoria libre
  void * mem_ptr;
  // Tamaño de esa memoria libre
  size_t size;
  // Estado del bloque
  STATE state;
}

/*
  Estructura del tipo Lista
*/
struct t_list {
  // Primer elemento de la lista
  struct Node head;
  // Puntero al comienzo de la memoria
  void * start_dir;
  // Tamaño total de la memoria
  size_t total_size;
}

// Redefinicion del puntero al tipo lista
typedef struct t_list * List;

Node first_fit(Node node, size_t size);

static List mem_blocks;

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
Node first_fit(Node node, size_t size){
  // En el caso de que no haya espacio o lleguemos al final de la lista
  if (node == null || node->size == 0){
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
      int space_left = node->size - size;

      // En el caso de que haya sobre la cantidad como para armar un nodo mas, lo aloca
      if (space_left > sizeof(Node)){
        // Calcula el puntero del nuevo nodo
        Node aux = (void *)((char *)node->mem_ptr + size);
        // Para no perder la referencia al siguiente
        aux->next = node->next;
        // Marca al nodo como libre
        aux->state = FREE;
        // Calcula el puntero a la memoria, sin contar el tamaño de este nodo
        aux->mem_ptr = (void *)((char *)aux + sizeof(Node));
        // Calcula el tamaño de la memoria porque se uso parte para el nodo
        aux->size = space_left - sizeof(Node);

        // Le cambia el tamaño al bloque porque se corto la cantidad de memoria que tiene
        node->size = size;
        node->next = aux;
      }

      // Lo marcamos como que NO esta libre
      node->state = NOT_FREE;

      return node;
    }
  }

  return first_fit(node->next, size);
}

/*
  Metodo para alocar memoria.
  Usa el algoritmo FIRST FIT, con una lista simplemente encadenada.
  Parametros:
    size_t size --> Tamaño de memoria para alocar
  Devuelve:
    void * --> Puntero a la direccion de memoria para empezar a alocar
*/
void * m_alloc(size_t size){
  Node node = first_fit(List->Head, size);
  if (ptr == NULL){
    return NULL;
  }
  return node->mem_ptr;
}

int m_free(void * ptr){
  // TODO
}

/*
  Metodo para inicializar la lista.
  Calcula el tamaño del primer nodo y de la lista, teniendo en cuenta el espacio que ocupa el
  puntero a la lista y al primer nodo.
  Parametros:
    void * start_dir --> Direccion donde comienza la memoria
    size_t total_size --> Tamaño de la memoria
*/
void init_list(void * start_dir, size_t total_size){
  // El puntero a la lista es donde comienza la memoria
  mem_blocks = start_dir;

  // Tamaño total de memoria
  mem_blocks->total_size = total_size;
  // Direccion de donde comienza toda la memoria
  mem_blocks->start_dir = start_dir;
  // Puntero al primer nodo, se calcula teniendo en cuenta el tamaño del puntero a la lista.
  mem_blocks->head = (void * ) ((char *)start_dir + sizeof(List));

  // Settea el siguiente en null porque es el primer bloque
  mem_blocks->Head->next = NULL;
  // Calcula el puntero a la memoria usando el puntero al primer nodo mas su tamaño
  mem_blocks->Head->mem_ptr = (void * ) ((char *)mem_blocks->Head + sizeof(Node));
  // Calcula el tamaño de la memoria para alocar, teniendo en cuenta el tamaño de la lista y el nodo ya alocados
  mem_blocks->Head->size = total_size - sizeof(List) - sizeof(Node);
  // Marca al nodo como libre
  mem_blocks->Head->state = FREE;
}
