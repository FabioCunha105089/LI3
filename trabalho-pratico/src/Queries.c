#include "Queries.h"
#include "Driver.h"
#include "Ride.h"
#include "User.h"
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "FileManager.h"
#include "ArrayList.h"
#include <math.h>


typedef struct query
{
    char id;
    char **args;
} Query;

static ArrayList *list = NULL;

void initListQuery(int size)
{
    if (!list)
        list = createAL(size, sizeof(Query *));
}

int getNArgs(char id)
{
    switch (id)
    {
    case '1':
    case '4':
        return 1;

    case '8':
        return 2;

    case '6':
        return 3;
    }
    return 0;
}

void loadQuery(char *sp)
{
    Query *query = (Query *)malloc(sizeof(Query));
    query->id = strsep(&sp, " ")[0];
    int nArgs = getNArgs(query->id);
    query->args = malloc(sizeof(char *) * nArgs);
    for (int i = 0; i < nArgs; i++)
    {
        query->args[i] = (char *)malloc(256);
        if (i == nArgs - 1)
        {
            query->args[i] = strsep(&sp, "\n");
            break;
        }
        query->args[i] = strsep(&sp, " ");
    }
    addAL(list, query);
}

double query4(char *city)
{
    return avgPayInCity(city);
}

double query6(char *city, char *dataA, char *dataB) {

    return avgDistanceInCityByDate(city, dataA, dataB);
}

char *query1_drivers(char *id)
{
    if (isDriverActive(id) == FALSE)
        return "";

    char *r = getDriverBasicInfo(id);

    if (doesDriverHaveRides(id) == FALSE)
    {
        strcat(r, "0.000;0;0.000");
        return r;
    }

    double *values = getDriverAvgScoreAndPay(id);
    char aux[100];
    sprintf(aux, "%.3f", values[0]);
    strcat(r, aux);
    strcat(r, ";");
    sprintf(aux, "%d", getNumberOfRidesDriver(id));
    strcat(r, aux);
    strcat(r, ";");
    sprintf(aux, "%.3f", values[1]);
    strcat(r, aux);
    free(values);
    return r;
}

char *query1_users(char *id)
{

    if (isUserActive(id) == FALSE)
        return "";

    char *r = getUserBasicInfo(id);

    if (doesUserHaveRides(id) == FALSE)
    {
        strcat(r, "0,000;0;0.000");
        return r;
    }

    double *values = getUserAvgScoreAndPay(id);
    char aux[100];
    sprintf(aux, "%.3f", values[0]);
    strcat(r, aux);
    strcat(r, ";");
    sprintf(aux, "%d", getNumberOfRidesUser(id));
    strcat(r, aux);
    strcat(r, ";");
    sprintf(aux, "%.3f", values[1]);
    strcat(r, aux);
    free(values);
    return r;
}

char *query1(char *id)
{
    return atoi(id) != 0 ? query1_drivers(id) : query1_users(id);
}

void executeQueries()
{
    int nQueries = getALSize(list);
    char aux[100];
    for (int i = 0; i < nQueries; i++)
    {
        Query *query = (Query *)getByIndex(list, i);
        switch (query->id)
        {
        case '1':
            output(query1(query->args[0]), i);
            break;

        case '4':
            sprintf(aux, "%.3lf", query4(query->args[0]));
            output(aux, i);
            break;
        case '6':
            sprintf(aux, "%.3f", query6(query->args[0], query->args[1], query->args[2]));
            output(aux, i);
            break;
        default:
            break;
        }
    }
}

void freeQuery()
{
    freeArrayList(list);
}
