#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include <glib.h>
typedef struct arraylist ArrayList;

ArrayList *createAL(int, int);
void addAL(ArrayList *, void *);
int getALSize(ArrayList*);
void *getByIndex(ArrayList*, int);
void freeArrayList(ArrayList*);
void quickSortArrayList(ArrayList *, int, int (*cmpFunc)(const void *, const void *));
#endif