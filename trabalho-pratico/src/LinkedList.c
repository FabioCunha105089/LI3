#include "LinkedList.h"
#include <stdlib.h>
#include <string.h>

typedef struct node{
    void* data;
    struct node* next;
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

void freeLinkedList(gpointer data)
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
    return;
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

int sizeOfLinkedList()
{
    return sizeof(LinkedList *);
}

ArrayList *LLtoALMultiple(LinkedList *lists, int elementSize, int nLists, int tSize)
{
    LinkedList *l;
    ArrayList *al = createAL(tSize, elementSize);
    int size;
    for(int i = 0; i < nLists; i++)
    {
        l = (LinkedList *) iterateLL(lists);
        size = getLLSize(l);
        for(int j = 0; j < size; j++)
            addAL(al, iterateLL(l));
    }
    return al;
}