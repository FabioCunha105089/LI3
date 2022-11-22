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

int getALSize(ArrayList* list){
    return list->size;
}

void* getByIndex(ArrayList* list, int index){
    return list->array[index];
}

void freeArrayList(ArrayList * list)
{
    for(int i = 0; i < list->size; i++)
    {
        free(list->array[i]);
    }
    free(list);
}

void quickSortArrayList(ArrayList *list, int elementSize, int (*cmpFunc)(const void *, const void *))
{
    qsort(list->array, list->size, elementSize, cmpFunc);
}