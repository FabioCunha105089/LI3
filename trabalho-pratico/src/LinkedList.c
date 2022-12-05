#include "LinkedList.h"
#include <stdlib.h>

typedef struct node{
    void* data;
    struct Node* next;
}Node;

typedef struct ll{
    Node* head;
    Node* tail;
    int size;
    Node *pointer;
}LinkedList;

LinkedList* createLL(){
    LinkedList* list = (LinkedList*) malloc (sizeof(LinkedList));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    list->pointer = NULL;
    return list;
}

Node* createNode(void* data){
    Node* node = (Node*) malloc (sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

gboolean isEmpty(LinkedList* list){
    return list->size == 0 ? TRUE : FALSE;
}

void addLL(LinkedList* list, void* data){
    Node* node = createNode(data);
    if(isEmpty(list) == TRUE){
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}

void* iterateLL(LinkedList* list){
    if(!list->pointer){
        list->pointer = list->head;
        return list->pointer->data;
    } else if(list->pointer == list->tail){
        list->pointer = list->head;
        return list->tail->data;
    }
    list->pointer = list->pointer->next;
    return list->pointer->data;
}

int getLLSize(LinkedList* list){
    return list->size;
}

GDestroyNotify freeLinkedList(gpointer data)
{
    LinkedList* list = (LinkedList *) data;
    Node* node;

    for(int i = 0; i < list->size; i++)
    {
        node = list->head;
        list->head = node->next;
        free(node);
    }
    free(list);
}

ArrayList *LLtoAL(LinkedList *ll, int elementSize)
{
    int size = getLLSize(ll);
    ArrayList *al = createAL(size, elementSize);
    for(int i = 0; i < size; i++)
    {
        addAL(al, iterateLL(ll));
    }
    return al;
}