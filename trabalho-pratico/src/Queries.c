#include "Queries.h"
#include "Driver.h"
#include "Ride.h"
#include "User.h"
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "FileManager.h"
#include "ArrayList.h"

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

void swap(char *a, char *b)
{
    char *temp = a;
    a = b;
    b = temp;
}

char *_q8getDriverID(char *s)
{
    char *sp = strdup(s);
    return strsep(&sp, ";");
}

char *_q8getUsername(char *s)
{
    char *sp = strdup(s);
    strsep(&sp, ";");
    strsep(&sp, ";");
    return strsep(&sp, ";");
}

char *_q8getRideID(char *s)
{
    char *sp = strdup(s);
    strsep(&sp, "?");
    return strsep(&sp, "\n");
}

int _q8CheckPivot(char *j, int pivot, char *high, int jay)
{
    return (getDriverAccAge(j) < pivot || getUserAccAge(_q8getUsername(j)) < getUserAccAge(_q8getUsername(high)) || atoi(_q8getRideID(j)) < atoi(_q8getRideID(high))) ? 0 : 1;
}

int partition(char **array, int low, int high)
{
    char *id = _q8getDriverID(array[high]);
    int pivot = getDriverAccAge(id);

    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (_q8CheckPivot(array[j], pivot, array[high], j) == 0)
        {
            i++;
            swap(array[i], array[j]);
        }
    }
    swap(array[i + 1], array[high]);

    return (i + 1);
}

void quickSort(char **array, int low, int high)
{
    if (low < high)
    {
        int pi = partition(array, low, high);

        quickSort(array, low, pi - 1);

        quickSort(array, pi + 1, high);
    }
}

char *_q8removeRideID(char *s)
{
    char *sp = strdup(s);
    return strsep(&sp, "?");
}

LinkedList *query8(char gender, int years)
{
    LinkedList *l = ridesByGenderAndAge(gender, years);
    int *size = (int *)malloc(sizeof(int));
    size = (int *)iterateLL(l);
    char **arr = malloc(sizeof(char *) * size[0]);
    arr = (char **)iterateLL(l);
    if(arr)
    {
        quickSort(arr, 0, size[0] - 1);
        for (int i = 0; i < size[0]; i++)
        {
            arr[i] = _q8removeRideID(arr[i]);
        }
    }

    free(l);
    LinkedList *r = createLL();
    addLL(r, arr);
    addLL(r, size);
    return r;
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
            sprintf(aux, "%.3f", query4(query->args[0]));
            output(aux, i);
            break;
        case '6':
            sprintf(aux, "%.3f", query6(query->args[0], query->args[1], query->args[2]));
            output(aux, i);
        case '8':
            outputMult(query8(query->args[0][0], atoi(query->args[1])), i);
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