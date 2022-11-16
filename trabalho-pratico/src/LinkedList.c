#include "LinkedList.h"
#include "glib.h"
#include <stdlib.h>

typedef struct node{
    void* data;
    struct Node* next;
}Node;

typedef struct ll{
    Node* head;
    Node* tail;
    int size;
}LinkedList;

LinkedList* createLL(){
    LinkedList* list = (LinkedList*) malloc (sizeof(LinkedList));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
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

void* findByIndex(LinkedList* list, int index){
    Node* node = list->head;
    while(index > 0){
        node = node->next;
        index--;
    }
    return node->data;
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