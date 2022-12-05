#include "FileManager.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>

int getLines(FILE *file)
{
    int size = 0;
    char c = fgetc(file);
    while (!feof(file))
    {
        if (c == '\n')
        {
            size++;
        }
        c = fgetc(file);
    }
    fseek(file, 0, SEEK_SET);
    return size;
}

int load(char *path, void *(*loadFunc)(char *), void (*initFunc)(int), int linesToSkip)
{
    FILE *file = fopen(path, "r");
    if (!file)
    {
        printf("Ficheiro %s nao encontrado.\n", path);
        return -1;
    }
    int size = getLines(file);
    initFunc(size);
    char *line = (char *)malloc(256);
    char *sp;
    for(int i = 0; i < linesToSkip; i++)
    {
        fgets(line, 256, file);
    }
        
    for (int i = 0; i < size; i++)
    {
        fgets(line, 256, file);
        sp = strdup(line);
        loadFunc(sp);
        free(sp);
    }
    free(line);
    fclose(file);
    return 0;
}

void output(char *r, int i) {
    
    char filename[100] = "./Resultados/command";
    char aux[100];
    sprintf(aux, "%d", i + 1);
    strcat(filename, aux);
    strcat(filename, "_output.txt");

    FILE *file = fopen(filename, "w+");   
    
    fputs(r, file);
    if(strcmp(r, "") != 0)
        fputs("\n", file);
    fclose(file);

}

void outputMult(LinkedList *r, int i)
{
    char **arr = (char **) iterateLL(r);
    int *size = (int *) iterateLL(r);

    char filename[100] = "./Resultados/command";
    char aux[100];
    sprintf(aux, "%d", i + 1);
    strcat(filename, aux);
    strcat(filename, "_output.txt");

    FILE *file = fopen(filename, "w+");

    if(*size == 0)
    {
        fputs("", file);
    } else {
        for(int i = 0; i < *size; i++)
        {
            if(!arr[i])
                break;
            fputs(arr[i], file);
            fputs("\n", file);
            free(arr[i]);
        }
    }
    free(size);
    free(arr);
    freeLinkedList(r);
}