#include "FileManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>

#include "Ride.h" //apagar depois
#include "Driver.h"
#include "Queries.h"
#include "User.h"

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

// int main(int argc, char const *argv[])
// {
//     ArrayList *rides = load("rides.csv", getElementSizeRide, loadRide);
//     LinkedList *hashs = getLLFromAL(rides, getRideHashTables);
//     ArrayList *drivers = load("drivers.csv", getElementSizeDriver, loadDriver);
//     ArrayList *users = load("users.csv", getElementSizeUser, loadUser);
//     char* r = query1("MMarques130",(GHashTable*) findByIndex(hashs, 1), (GHashTable*) findByIndex(hashs, 2), drivers, users);
//     printf(r);
//     return 0;
// }
