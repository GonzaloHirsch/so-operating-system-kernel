/*
    Buddy memory manager.
    Basado en la unica e inigualable: https://en.wikipedia.org/wiki/Buddy_memory_allocation
    
    Para almacenar memoria actua del siguiente modo: 
    Se almacena primero el buddyTree, luego los nodos y finalmente la memoria total solicitada
    Queda de la siguente manera la memoria(para un caso con total de 3 niveles):
    |BuddyTree
    |Nodo N0
    |Nodo N1 Left de N0
    |Nodo N2 Left de N1 Left de N0
    |Nodo N2 Right de N1 Left de N0
    |Nodo N1 Right de N0
    |Nodo N2 Left de N1 Right de N0
    |Nodo N2 Right de N1 Left de N0
    |MEM 
    |MEM
    |MEM
    |MEM
    |....mas MEM
*/
#include <buddyManager.h>
#include <stdint.h>
#include <stddef.h>

//Tipo nodo.
typedef struct nodeType{

    struct nodeType * left;
    struct nodeType * right;
    void * memPtr;
    size_t size;
    enum BUDDY_STATE state; //3 estados posibles: FREE, NOT_FREE y PARENT(dividido)
    int level;

} buddy_node_t;

//Arbol de buddies
typedef struct treeType{

    buddy_node_t * head;
    void * startDir;
    size_t totalSize;          //Total size available to be allocated
    size_t smallestSize;       //Minimum size of the block
    int numberOfLevels;


} buddy_tree_t;

static buddy_tree_t * buddyTree;


void * reserveMemory(size_t newSize, buddy_node_t * node){

    void * toReturn;

    //Primero caso: esta ocupado, no sirve.
    if(node->state == B_NOT_FREE){
        return NULL;
    }

    //Segundo caso: es un padre, lo delego a sus hijos.
    else if(node->state == B_PARENT){
        //Probamos con el hijo izquierdo
        toReturn = reserveMemory(newSize,node->left);
        if(toReturn == NULL){
            //Probamos con el hijo derecho
            return reserveMemory(newSize, node->right);
        }
        return toReturn;
    }

    //Tercer caso: esta libre, vemos si lo podemos dividir.
    //Chequeamos que halla espacio disponible para lo que queremos allocar.
    else if(node->state == B_FREE && newSize <= node->size ){

        //Si no podemos dividirlo mas(estamos en el menor size posible del nodo)
        //Lo allocamos.
        if(node->level == buddyTree->numberOfLevels){
            node->state = B_NOT_FREE;
            return node->memPtr;
        }
        //Si entra en uno de los nodos hijo, dividimos el nodo actual,
        //creamos ambos y lo mandamos al de la izquierda
        else if(newSize <= node->size / 2){
            
            //Creamos los hijos en caso que nunca hayan sido creados.
            //Puede haber pasado que hayan sido creados y liberados anteriormente...
            if(node->left == NULL || node->right ==NULL){
                buddy_node_t * auxL = node+sizeof(buddy_node_t);
                auxL->size = node->size/2;
                auxL->left = auxL->right = NULL;
                auxL->memPtr = node->memPtr;
                auxL->state = B_FREE;
                auxL->level = node->level-1;

                //El nodo de la derecha va a estar despues de todos los nodos hijos del nodo de la izquierda).
                //Si se le ocurre una mejor forma de organizarlos digamela.
                buddy_node_t * auxR = node + (powerTo(buddyTree->numberOfLevels - node->level + 1 , 2) - 1) * sizeof(buddy_node_t);
                auxR->size = node->size/2;
                auxR->left = auxR->right = NULL;
                auxR->memPtr = (void *)((char *) node->memPtr + node->size/2);
                auxR->state = B_FREE;
                auxR->level = node->level-1;

            //Asignamos ambos nodos hijos
            node->left = auxL;
            node->right = auxR;

            }
            // Lo cambiamos a estado parent. Va a ser papa!!!!. Allocamos en el nodo de la izquierda.
            node->state = B_PARENT;
            return reserveMemory(newSize, node->left);
    

        }

        //Si solo entra en el tamanio del nodo actual y no en sus hijos
        else{
            node->state = B_NOT_FREE;
            return node->memPtr;
        }
    }

    return NULL;
}

void * buddyMalloc(size_t newSize){
    return reserveMemory(newSize, buddyTree->head);
}

/* Funcion recursiva para liberar memoria
   Returns: 1 si lo libero, 0 sino.
*/
int buddyFreeMemory(void * ptr, buddy_node_t * node){

    int result;

    //Por las dudas.
    if(node == NULL){
        return 0;
    }

    //Caso 1: Esta libre: vuelvo.
    if(node->state == B_FREE){
        return 0;
    }
    //Caso 2: Es padre: lo delego a sus hijos
    //En caso que ambos esten libres luego, trato de mergearlos.
    else if(node->state == B_PARENT){
        result = buddyFreeMemory(ptr,node->left);
        if(result == 0){
            result = buddyFreeMemory(ptr,node->right);
        }

        //Vemos si puedo mergear a los hijos.
        if(node->left->state == B_FREE && node->right->state == B_FREE){
            node->state = B_FREE;
            //No hay necesidad de destruirlos, capaz los usamos mas adelante.
        }

        return result;
    }
    //Caso 3: Esta ocupado: veo si es la direccion que estoy buscando liberar.
    else{
        //Caso que sea la direccion deseada: liberamos el nodo.

        if(node->memPtr == ptr){
            node->state=B_FREE;
            return 1;
        }
        //Si no es la direccion que estoy buscando, simplemente vuelvo.
        else{
            return 0;
        }

    }
    return 0;
}


int buddyFree(void * ptr){
    return buddyFreeMemory(ptr, buddyTree->head);
}


void initializeBuddyMemory(void * startDir, size_t totalSize){

    if(totalSize<SMALLEST_SIZE) totalSize=SMALLEST_SIZE;

    size_t base = logBase2Ceil(totalSize); //El tamanio tiene que ser una potencia de 2
    totalSize = powerTo(2,base);

    size_t lowestLevelNodeCant = totalSize/SMALLEST_SIZE;
    size_t maxNodeCant = lowestLevelNodeCant* 2 - 1;    //Cantidad maxima de nodos que podemos tener.  

    buddyTree = startDir;
    //El tamanio total sera de el size requerido, mas el tamanio del tree mas el tamanio de todos los nodos posibles.
    //(el tamanio de los nodos esta reservado no los alocaremos hasta que sea necesario)
    buddyTree->totalSize = totalSize + sizeof(buddy_tree_t)+ maxNodeCant *  sizeof(buddy_node_t);
    buddyTree->smallestSize = SMALLEST_SIZE;
    buddyTree->head=(void *)((char *)startDir + sizeof(buddy_tree_t));
    buddyTree->startDir = startDir;
    buddyTree->numberOfLevels = logBase2Ceil(lowestLevelNodeCant);

    buddyTree->head->left=NULL;
    buddyTree->head->right=NULL;
    buddyTree->head->size=totalSize;
    //El puntero a la memoria en si, arranca despues de la estructura de tree y la estructura de nodos.
    buddyTree->head->memPtr = (void *)((char *)buddyTree->startDir + sizeof(buddy_tree_t)+ maxNodeCant * sizeof(buddy_node_t));
    buddyTree->head->state = B_FREE;
    buddyTree->head->level = 0;


}

//HELPERS

size_t powerTo(size_t value,size_t pow){
    if(pow == 0){
        return 1;
    }

    int result = 1;
    while(pow > 0){
        result *= value;
        pow--;

    }

    return result;
}

//Devuelve la potencia tomando el techo del resultado
size_t logBase2Ceil(size_t value){
    int i = 0;
    while((int) powerTo(2,i) < value){
        i++;
    }
    return i;
}




