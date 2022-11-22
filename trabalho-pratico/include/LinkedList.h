#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
    #include <glib.h>
    #include "ArrayList.h"
    typedef struct node Node;
    typedef struct ll LinkedList;

    LinkedList* createLL();
    gboolean isEmpty(LinkedList*);
    void addLL(LinkedList*, void*);
    void* iterateLL(LinkedList*);
    int getLLSize(LinkedList*);
    GDestroyNotify freeLinkedList(gpointer);
    ArrayList *LLtoAL(LinkedList *, int);

#endif
