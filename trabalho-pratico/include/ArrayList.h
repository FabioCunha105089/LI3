#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_

    #include <glib.h>
    typedef struct arraylist* ArrayList;

    ArrayList* createAL(int, int);
    void addAL(ArrayList*, void*);
    void* find(ArrayList* , void* , void* (*findFunc)(void**, int, void*));
    GHashTable* arrayListToHashMap(ArrayList*, GHashTable* (*hashTableFunc)(void**, int));
#endif