#include "FileManager.h"
#include "Driver.h"
#include "User.h"
#include "Ride.h"
#include "Queries.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool doLoads(char const *csvPath)
{
    char aux[50];
    strcpy(aux, csvPath);
    int check = load(strcat(aux, "/drivers.csv"), loadDriver, initListDriver, 1);
    if (check == -1) return false;
    
    printf("Drivers loaded = %d\n", check);
    strcpy(aux, csvPath);
    check = load(strcat(aux, "/users.csv"), loadUser, initListUser, 1);
    if (check == -1)
    {
        freeDriver();
        return false;
    };
    printf("Users loaded = %d\n", check);
    strcpy(aux, csvPath);
    check = load(strcat(aux, "/rides.csv"), loadRide, initListRide, 1);
    if (check == -1)
    {
        freeDriver();
        freeUser();
        return false;
    }
    printf("Rides loaded = %d\n", check);
    updateRide(check);
    return true;
}

int batchMode(char const *argv[])
{
    bool check = true;
    check = doLoads(argv[1]);
    if (!check)
        return 0;
    initHashTables();
    if (!load(argv[2], loadQuery, initListQuery, 0))
    {
        freeDriver();
        freeUser();
        freeRide();
        return 0;
    }
    executeQueries();
    freeQuery();
    freeDriver();
    freeUser();
    freeRide();
    return 0;
}

int interactiveMode()
{
    system("clear");
    char csvPath[256];
    int check = -1, nArgs;
    char choice = 'a';
    char **args;
    do
    {
        printf("Grupo 61\nInsira o caminho para a pasta dos ficheiros de entrada:\n");
        fgets(csvPath, 256, stdin);
        size_t ln = strlen(csvPath) - 1;
        if (csvPath[ln] == '\n')
            csvPath[ln] = '\0';
        system("clear");
        printf("Carregando...\n");
        check = doLoads(csvPath);
    } while (check == -1);
    initHashTables();
    system("clear");

    while (choice != '0')
    {
        printf("Qual query quer realizar? (0 para sair)\n");
        choice = getchar();
        fgets(csvPath, 256, stdin); // BUFFER
        nArgs = getNArgs(choice);
        args = (char **)malloc(sizeof(char *) * nArgs);
        for (int i = 0; i < nArgs; i++)
        {
            args[i] = (char *)malloc(50);
            printf("Insira o %dº argumento:\n", i + 1);
            fgets(args[i], 50, stdin);
            size_t ln = strlen(args[i]) - 1;
            if (args[i][ln] == '\n')
                args[i][ln] = '\0';
        }
        executeQuery(choice, args);
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    argc == 3 ? batchMode(argv) : interactiveMode();
    return 0;
}