#include "FileManager.h"
#include "Driver.h"
#include "User.h"
#include "Ride.h"
#include "Queries.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool loadData(char const *csvPath)
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
    printf("Rides loaded = %d\n\n", check);
    updateRide(check);
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