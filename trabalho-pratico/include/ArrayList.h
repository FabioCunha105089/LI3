#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_

    typedef struct arraylist* ArrayList;

    ArrayList* create(int, int);
    void add(ArrayList*, void*);
    void* find(ArrayList* , void* , void* (*findFunc)(void**, int, void*));

#endif