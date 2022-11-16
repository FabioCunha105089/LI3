#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
    #include <glib.h>
    typedef struct node Node;
    typedef struct ll LinkedList;

    LinkedList* createLL();
    gboolean isEmpty(LinkedList*);
    void addLL(LinkedList*, void*);
    void* findByIndex(LinkedList*, int);
    int getLLSize(LinkedList*);
    GDestroyNotify freeLinkedList(gpointer);

#endif
