#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
    #include "boolean.h"

    typedef struct node* Node;
    typedef struct ll* LinkedList;

    LinkedList* createLL();
    boolean isEmpty(LinkedList*);
    void addLL(LinkedList*, void*);
    void* findByIndex(LinkedList*, int);
    int getLLSize(LinkedList*);

#endif