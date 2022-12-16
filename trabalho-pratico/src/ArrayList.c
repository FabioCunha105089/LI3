#include <stdlib.h>
#include "ArrayList.h"
#include <string.h>

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

void freeArrayList(ArrayList * list, void (*freeFunc)(void *))
{
    for(int i = 0; i < list->size; i++)
    {
        if(list->array[i])
            freeFunc(list->array[i]);
    }
    free(list->array);
    free(list);
}

void freeArrayListSimple(ArrayList* list)
{
    free(list->array);
    free(list);
}

void quickSortArrayList(ArrayList *list, int elementSize, int (*cmpFunc)(const void *, const void *))
{
    qsort(list->array, list->size, elementSize, cmpFunc);
}

ArrayList *copyAL(ArrayList *a, int elementSize)
{
    ArrayList *b = createAL(a->size, elementSize);
    int k = 0;
    for(int i = 0; i < a->size; i++)
    {
        if(a->array[i])
        {
            memcpy(&b->array[k], &a->array[i], elementSize);
            k++;
        }
    }
    return b;
}

void updateArrayList(ArrayList *list, int elementSize, int newSize)
{
    realloc(list->array, elementSize * newSize);
    list->size = newSize;
}