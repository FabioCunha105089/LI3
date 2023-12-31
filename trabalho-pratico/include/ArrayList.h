#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

typedef struct arraylist ArrayList;

ArrayList *createAL(int, int);
void addAL(ArrayList *, void *);
void addAtIndex(ArrayList *, void *, int);
int getALSize(ArrayList*);
void *getByIndex(ArrayList*, int);
void freeArrayList(ArrayList * list, void (*freeFunc)(void *));
void quickSortArrayList(ArrayList *, int, int (*cmpFunc)(const void *, const void *));
void freeArrayListSimple(ArrayList*);
ArrayList *copyAL(ArrayList *, int);
void updateArrayList(ArrayList *, int, int);
#endif