#include <stdlib.h>
#include "boolean.h"
#include "ArrayList.h"

typedef struct arraylist{
    void** array;
    int size;
    int pointer;
}ArrayList;

ArrayList* createAR(int size, int elementSize){
    if(size > 0){
        ArrayList* list = (ArrayList*) malloc (sizeof(ArrayList));
        list->array = (void*) malloc (elementSize * size);
        list->size = size;
        list->pointer = 0;
        return list;
    }
    return NULL;
}
void addAR(ArrayList *list, void *input) {
    list->array[list->pointer] = input;
    list->pointer++;
}

void* find(ArrayList* list, void* input, void* (*findFunc)(void**, int, void*))
{
    return (*findFunc)(list->array, list->size, input);
}

GHashTable* arrayListToHashMap(ArrayList* list, GHashTable* (*hashTableFunc)(void**, int)){
    return (*hashTableFunc)(list->array, list->size);
}