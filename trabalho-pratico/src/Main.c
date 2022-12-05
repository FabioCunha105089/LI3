#include "FileManager.h"
#include "Driver.h"
#include "User.h"
#include "Ride.h"
#include "Queries.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int batchMode(char const *argv[])
{   
    char *csvPath = strdup(argv[1]);
    load(strcat(csvPath, "/drivers.csv"), loadDriver, initListDriver, 1);
    free(csvPath);
    csvPath = strdup(argv[1]);
    load(strcat(csvPath, "/users.csv"), loadUser, initListUser, 1);
    free(csvPath);
    csvPath = strdup(argv[1]);
    load(strcat(csvPath, "/rides.csv"), loadRide, initListRide, 1);
    initHashTables();
    load(argv[2], loadQuery, initListQuery, 0);
    executeQueries();
    freeDriver();
    freeUser();
    freeQuery();
    freeRide();
    free(csvPath);
    return 0;
}

int interactiveMode()
{
    system("clear");
    char csvPath[256];
    int check = 1, nArgs;
    char choice = 'a';
    char *aux, **args;
    do
    {
        printf("Grupo 61\nInsira o caminho para a pasta dos ficheiros de entrada:\n");
        fgets(csvPath, 256, stdin);
        size_t ln = strlen(csvPath) - 1;
        if (csvPath[ln] == '\n')
            csvPath[ln] = '\0';
        system("clear");
        aux = strdup(csvPath);
        system("clear");
        printf("Carregando...\n");
        check = load(strcat(aux, "/drivers.csv"), loadDriver, initListDriver, 1);
        if (check != -1)
        {
            free(aux);
            aux = strdup(csvPath);
            check = load(strcat(aux, "/users.csv"), loadUser, initListUser, 1);
            if (check != -1)
            {
                free(aux);
                aux = strdup(csvPath);
                check = load(strcat(aux, "/rides.csv"), loadRide, initListRide, 1);
            }
        }
    } while (check == -1);
    initHashTables();
    system("clear");

    while (choice != '0')
    {
        printf("Qual query quer realizar? (0 para sair)\n");
        choice = getchar();
        fgets(csvPath, 256, stdin); //BUFFER
        nArgs = getNArgs(choice);
        args = (char **) malloc(sizeof(char *) * nArgs);
        for (int i = 0; i < nArgs; i++)
        {
            args[i] = (char *) malloc(50);
            printf("Insira o %dº argumento:\n", i + 1);
            fgets(args[i], 50, stdin);
            size_t ln = strlen(args[i]) - 1;
            if (args[i][ln] == '\n')
            args[i][ln] = '\0';
        }
        executeQuery(choice, args);
    }
    freeDriver();
    freeUser();
    freeRide();
    free(aux);
    return 0;
}

int main(int argc, char const *argv[])
{
    argc == 3 ? batchMode(argv) : interactiveMode();
}