/*
    Buddy memory manager.
    Basado en la unica e inigualable: https://en.wikipedia.org/wiki/Buddy_memory_allocation
    
    NOTA: el size requerido obviamente va a ser para uso total, por lo que abarca el size
    usado por node y tree.
    Por lo tanto el totalSize del tree(es decir de todo el manager) sera el size deseado por
    usuario mas el size requerido para alocar la estructura del tree.
    El totalsize del nodo original sera el sizetotal requerido por el usuario.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <buddyManager.h>

//Tipo nodo.
typedef struct nodeType{

    struct node * left;
    struct node * right;
    void * memPtr;
    size_t size;
    enum STATE state; //3 estados posibles: FREE, NOT_FREE y PARENT(dividido)

} buddy_node_t;

//Arbol de buddies
typedef struct treeType{

    buddy_node_t * head;
    void * startDir;
    size_t totalSize;          //Total size available to be allocated
    size_t smallestSize;       //Minimum size of the block


} buddy_tree_t;

static buddy_tree_t * buddyTree;

void * reserveMemory(size_t newSize, buddy_node_t * node){

    void * toReturn;

    //Primero caso: esta ocupado, no sirve.
    if(node->state == NOT_FREE){
        return NULL;
    }

    //Segundo caso: es un padre, lo delego a sus hijos.
    else if(node->state == PARENT){
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
    else if(node->state == FREE && newSize + sizeof(buddy_node_t) <= node->size ){

        //Si no podemos dividirlo mas(estamos en el menor size posible del nodo)
        //Lo allocamos
        if(node->size == buddyTree->smallestSize){
            node->state = NOT_FREE;
            return node->memPtr;
        }
        //Si entra en uno de los nodos hijo, dividimos el nodo actual,
        //creamos ambos y lo mandamos al de la izquierda
        else if(newSize+sizeof(buddy_node_t) <= node->size / 2){
            
            //TODO:
            //Como pija mirar lo de donde guardamos el nodo padre para que la distribucion
            // del espacio sea equitativo.


            buddy_node_t * auxL = node->memPtr;
            auxL->size = node->size/2;
            auxL->left = auxL->right = NULL;
            auxL->memPtr = (void *)((char *) node->memPtr + sizeof(buddy_node_t));
            auxL->state = FREE;

            buddy_node_t * auxR = node->memPtr;
            auxR->size = node->size/2;
            auxR->left = auxR->right = NULL;
            auxR->memPtr = (void *)((char *) node->memPtr + sizeof(buddy_node_t));
            auxR->state = FREE;

        }

        //Si solo entra en el tamanio del nodo actual y no sus hijos
        else{
            node->state = NOT_FREE;
            return node->memPtr;
        }
    }

    return NULL;
}

void initializeBuddyMemory(void * startDir, size_t totalSize){

    if(totalSize<SMALLEST_SIZE) totalSize=SMALLEST_SIZE;
    int base = (int) ceil(log2(totalSize)); //El tamanio tiene que ser una potencia de 2

    buddyTree = startDir;
    buddyTree->totalSize = 2^base + sizeof(buddy_tree_t);
    buddyTree->smallestSize = SMALLEST_SIZE;
    buddyTree->head=(void *)((char *)startDir + sizeof(buddy_tree_t));
    buddyTree->startDir = startDir;

    buddyTree->head->left=NULL;
    buddyTree->head->right=NULL;
    buddyTree->head->size=buddyTree->totalSize - sizeof(buddy_tree_t); //cuidado con el size que ocupa los nodos y el tree.
    buddyTree->head->memPtr = (void *)((char *)buddyTree->head + sizeof(buddy_node_t));
    buddyTree->head->state = FREE;

}





