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
int free_memory(Node actual, Node previous, void * ptr);

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
  Node node = first_fit(List->head, size);
  if (ptr == NULL){
    return NULL;
  }
  return node->mem_ptr;
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
int free_memory(Node actual, Node previous, void * ptr){
  // En el caso de que lleguemos al final o que nos pasen un puntero nulo
  if (actual == NULL || ptr == NULL){
    return 0;
  }

  // En este caso encontramos al puntero que buscamos
  if (actual->mem_ptr == ptr){
    // Caso de ANTERIOR --> OCUPADO o NULL y SIGUIENTE --> OCUPADO o NULL
    if ((previous == NULL || (previous != NULL && previous->state == NOT_FREE)) && ((actual->next != NULL && actual->next->state == NOT_FREE) || actual->next == NULL)){
      actual->state = FREE;
      return 1;
    }
    // Caso de ANTERIOR --> LIBRE y SIGUIENTE --> OCUPADO o NULL
    else if (previous != NULL && previous->state == FREE && ((actual->next != NULL && actual->next->state == NOT_FREE) || actual->next == NULL)){
      previous->size = previous->size + actual->size + sizeof(Node);
      previous->next = actual->next;
      return 1;
    }
    // Caso de ANTERIOR --> LIBRE y SIGUIENTE --> LIBRE
    else if (previous != NULL && actual->next != NULL && previous->state == FREE && actual->next->state == FREE){
      previous->size = previous->size + actual->size + actual->next->size + (2 * sizeof(Node));
      previous->next = actual->next->next;
      return 1;
    }
    // Caso de ANTERIOR --> OCUPADO o NULL y SIGUIENTE --> LIBRE
    else if ((previous == NULL || (previous != NULL && actual->next != NULL)) && previous->state == NOT_FREE && actual->next->state == FREE){
      actual->size = actual->size + actual->next->size + sizeof(Node);
      actual->next = actual->next->next;
      return 1;
    } else {
      return 0;
    }
  } else {
    return free_memory(actual->next, actual, ptr);
  }
}

/*
  Metodo para liberar la memoria.
  Parametros:
    void * ptr --> Puntero a liberar
  Devuelve:
    int --> 1 si fue exitoso, 0 si no lo fue
*/
int m_free(void * ptr){
  int result = free_memory(List->head, NULL, ptr);
  return result;
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
  mem_blocks->head->next = NULL;
  // Calcula el puntero a la memoria usando el puntero al primer nodo mas su tamaño
  mem_blocks->head->mem_ptr = (void * ) ((char *)mem_blocks->head + sizeof(Node));
  // Calcula el tamaño de la memoria para alocar, teniendo en cuenta el tamaño de la lista y el nodo ya alocados
  mem_blocks->head->size = total_size - sizeof(List) - sizeof(Node);
  // Marca al nodo como libre
  mem_blocks->head->state = FREE;
}
