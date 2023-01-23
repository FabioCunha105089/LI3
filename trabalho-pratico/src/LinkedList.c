#include "LinkedList.h"
#include <stdlib.h>
#include <string.h>

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

bool isEmpty(LinkedList* list){
    return list->size == 0;
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
        return list->pointer->data;
    }
    list->pointer = list->pointer->next;
    return list->pointer->data;
}

int getLLSize(LinkedList* list){
    if(!list)
        return 0;
    return list->size;
}

GDestroyNotify freeLinkedList(void *data)
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

void addStringIfNotIn(LinkedList *list, char *input)
{
    Node *node = list->head;
    for(int i = 0; i < list->size; i++)
    {
        if(strcmp((char *) node->data, input) == 0)
            return;
        node = node->next;
    }
    addLL(list, input);
}

int getIndexFromLL(LinkedList *list, char *input)
{
    Node *node = list->head;
    for(int i = 0; i < list->size; i++)
    {
        if(strcmp((char *) node->data, input) == 0)
            return i;
        node = node->next;
    }
    return 0;
}