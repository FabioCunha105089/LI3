#include "FileManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int getLines(FILE* file){
    int size = 0;
    char c = fgetc(file);
    while(!feof(file)){
        if(c == '\n'){
            size++;
        }
        c = fgetc(file);
    }
    fseek(file, 0, SEEK_SET);
    return size - 1;
}

ArrayList* load(char* path,int (*getElementSizeFunc)(), void* (*loadFunc)(char*)){
    FILE* file = fopen(path, "r");
    if(!file){
        printf("Ficheiro nao encontrado.");
        exit(-1);
    }
    int size = getLines(file);
    char* line = (char*)malloc(256);
    char* sp;
    fgets(line, 256, file);
    ArrayList* list = create(size, (*getElementSizeFunc)());
    for(int i = 0; i < size; i++){
        fgets(line, 256, file);
        sp = strdup(line);
        add(list, (*loadFunc)(sp));
    }
    free(list);
    free(sp);
    fclose(file);
    return list;
}