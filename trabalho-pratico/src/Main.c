#include "FileManager.h"
#include "Driver.h"
#include "User.h"
#include "Ride.h"
#include "Queries.h"
#include <string.h>

int batchMode(char const *argv[])
{
    char *csvPath = argv[1];
    char *aux = strdup(csvPath);
    load(strcat(aux, "/drivers.csv"), loadDriver, initListDriver, 1);
    aux = strdup(csvPath);
    load(strcat(aux, "/users.csv"), loadUser, initListUser, 1);
    aux = strdup(csvPath);
    load(strcat(aux, "/rides.csv"), loadRide, initListRide, 1);
    initHashTables();
    load(argv[2], loadQuery, initListQuery, 0);
    executeQueries();
    freeDriver();
    freeUser();
    freeQuery();
    freeRide();
    free(aux);
    return 0;
}

int main(int argc, char const *argv[])
{
    return argc == 3 ? batchMode(argv) : 0;
}