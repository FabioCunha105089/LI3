#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include <glib.h>
#include "LinkedList.h"
typedef struct arraylist ArrayList;

ArrayList *createAL(int, int);
void addAL(ArrayList *, void *);
void *find(ArrayList *, void *, void *(*findFunc)(void **, int, void *));
GHashTable *arrayListToHashMap(ArrayList *, GHashTable *(*hashTableFunc)(void **, int));
LinkedList *getLLFromAL(ArrayList *, LinkedList *(*llFunc)(void **, int));
int getALSize(ArrayList*);
#endif