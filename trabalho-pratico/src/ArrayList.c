#include <stdlib.h>
#include "boolean.h"
#include "ArrayList.h"

typedef struct arraylist{
    void** array;
    int size;
    int elementSize;
    int pointer;
}ArrayList;

ArrayList* create(int size, int elementSize){
    if(size > 0){
        ArrayList* list = (ArrayList*) malloc (sizeof(ArrayList));
        list->array = (void*) malloc (elementSize * size);
        list->size = size;
        list->pointer = 0;
        list->elementSize = elementSize;
        return list;
    }
    return NULL;
}

void add(ArrayList *list, void *input) {
    list->array[list->pointer] = input;
    list->pointer++;
}

void* find(ArrayList* list, void* input, void* (*findFunc)(void**, int, void*))
{
    return (*findFunc)(list->array, list->size, input);
}