#include "FileManager.h"
#include <stdio.h>
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
    return size - 1;
}

void load(char *path, void *(*loadFunc)(char *), void (*initFunc)(int))
{
    FILE *file = fopen(path, "r");
    if (!file)
    {
        printf("Ficheiro nao encontrado.");
        exit(-1);
    }
    int size = getLines(file);
    initFunc(size);
    char *line = (char *)malloc(256);
    char *sp;
    fgets(line, 256, file);
    for (int i = 0; i < size; i++)
    {
        fgets(line, 256, file);
        sp = strdup(line);
        loadFunc(sp);
    }
    free(line);
    fclose(file);
}

void output(char *r, int i) {
    
    char aux1[100];
    sprintf(aux1, "%d", i);
    aux1 = strcat("../Resultados/command", aux1);
    char *filename = strcat(aux2, "_output.txt");

    FILE *file = fopen(filename, "w");
    
    if (!file) {
        printf("Ficheiro nao encontrado");
        exit(-1);
    }

    fprintf(file, r);
    fclose(file);

}