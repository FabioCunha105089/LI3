#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
    #include <glib.h>
    #include "ArrayList.h"
    #include <stdbool.h>
    typedef struct node Node;
    typedef struct ll LinkedList;

    LinkedList* createLL();
    bool isEmpty(LinkedList*);
    void addLL(LinkedList*, void*);
    void* iterateLL(LinkedList*);
    int getLLSize(LinkedList*);
    GDestroyNotify freeLinkedList(gpointer);
    ArrayList *LLtoAL(LinkedList *, int);
    void addStringIfNotIn(LinkedList *, char *);
    int getIndexFromLL(LinkedList *, char *);

#endif
