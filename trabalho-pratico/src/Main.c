#include "FileManager.h"
#include "Driver.h"
#include "User.h"
#include "Ride.h"
#include "Queries.h"
#include <string.h>

int batchMode(char const *argv[])
{
    char *csvPath = argv[0];
    load(strcat(strdup(csvPath), "/drivers.csv"), loadDriver, initListDriver);
    load(strcat(strdup(csvPath), "/users.csv"), loadUser, initListUser);
    load(strcat(strdup(csvPath), "/rides.csv"), loadRide, initListRide);
    initHashTables();
    load(argv[1], loadQuery, initListQuery);
    executeQueries();
    return 0;
}

int main(int argc, char const *argv[])
{
    return argc == 2 ? batchMode(argv) : 0;
}