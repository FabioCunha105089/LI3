#include <stdlib.h>
#include "ArrayList.h"

typedef struct arraylist
{
    void **array;
    int size;
    int pointer;
} ArrayList;

ArrayList *createAL(int size, int elementSize)
{
    if (size > 0)
    {
        ArrayList *list = (ArrayList *)malloc(sizeof(ArrayList));
        list->array = (void *)malloc(elementSize * size);
        list->size = size;
        list->pointer = 0;
        return list;
    }
    return NULL;
}
void addAL(ArrayList *list, void *input)
{
    list->array[list->pointer] = input;
    list->pointer++;
}

void *find(ArrayList *list, void *input, void *(*findFunc)(void **, int, void *))
{
    return findFunc(list->array, list->size, input);
}

GHashTable *arrayListToHashMap(ArrayList *list, GHashTable *(*hashTableFunc)(void **, int))
{
    return hashTableFunc(list->array, list->size);
}

LinkedList *getLLFromAL(ArrayList *al, LinkedList *(*llFunc)(void **, int))
{
    return llFunc(al->array, al->size);
}

int getALSize(ArrayList* list){
    return list->size;
}

void* getByIndex(ArrayList* list, int index){
    return list->array[index];
}