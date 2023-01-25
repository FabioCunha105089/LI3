#include "FileManager.h"
#include "Driver.h"
#include "User.h"
#include "Ride.h"
#include "Queries.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool loadData(char *csvPath)
{
    char *aux = strdup(csvPath);
    int check = load(strcat(aux, "/drivers.csv"), loadDriver, initListDriver, 1);
    if (check == -1)
    {
        free(aux);
        return false;
    }
    printf("Drivers loaded = %d\n", check);
    free(aux);
    aux = strdup(csvPath);
    check = load(strcat(aux, "/users.csv"), loadUser, initListUser, 1);
    printf("Users loaded = %d\n", check);
    if (check == -1)
    {
        freeDriver();
        free(aux);
        return false;
    };
    free(aux);
    aux = strdup(csvPath);
    check = load(strcat(aux, "/rides.csv"), loadRide, initListRide, 1);
    if (check == -1)
    {
        freeDriver();
        freeUser();
        free(aux);
        return false;
    }
    printf("Rides loaded = %d\n\n", check);
    updateRide(check);
    free(aux);
    return true;
}

int testMode(char const *argv[])
{
    bool check = true;
    check = loadData(argv[1]);
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
    executeTests(argv[3]);
    freeQuery();
    freeDriver();
    freeUser();
    freeRide();
    return 0;
}

int main(int argc, char const *argv[])
{
    return argc == 4 ? testMode(argv) : 0;
}