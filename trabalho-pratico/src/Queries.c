#include "Queries.h"
#include "Driver.h"
#include "Ride.h"
#include "User.h"
#include <stdlib.h>
#include <string.h>
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
    case '2':
    case '3':
        return 1;

    case '7':
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
            query->args[i] = strdup(strsep(&sp, "\n"));
            break;
        }
        query->args[i] = strdup(strsep(&sp, " "));
    }
    addAL(list, query);
}

char *query1_drivers(char *id)
{
    if (isDriverActive(id) == false)
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

    if (isUserActive(id) == false)
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

LinkedList *query2(int n)
{
    char **r = topNdrivers(n);
    LinkedList *l = createLL();
    addLL(l, r);
    int *s = (int *)malloc(sizeof(int));
    *s = n;
    addLL(l, s);
    return l;
}

LinkedList *query3(int n)
{
    char **r = mostDistUsers(n);
    LinkedList *l = createLL();
    addLL(l, r);
    int *s = (int *)malloc(sizeof(int));
    *s = n;
    addLL(l, s);
    return l;
}

double query4(char *city)
{
    return avgPayInCity(city);
}

double query6(char *city, char *dataA, char *dataB)
{

    return avgDistanceInCityByDate(city, dataA, dataB);
}

LinkedList *query7(int n, char *city)
{
    char **r = driversByScoreInCity(city, n);
    LinkedList *l = createLL();
    addLL(l, r);
    int *s = (int *)malloc(sizeof(int));
    *s = n;
    addLL(l, s);
    return l;
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
            free(query->args[0]);
            break;
        case '2':
            outputMult(query2(atoi(query->args[0])), i);
            free(query->args[0]);
            break;
        case '3':
            outputMult(query3(atoi(query->args[0])), i);
            free(query->args[0]);
            break;
        case '4':
            sprintf(aux, "%.3lf", query4(query->args[0]));
            output(aux, i);
            free(query->args[0]);
            break;
        case '6':
            sprintf(aux, "%.3f", query6(query->args[0], query->args[1], query->args[2]));
            output(aux, i);
            free(query->args[0]);
            free(query->args[1]);
            free(query->args[2]);
            break;
        case '7':
            outputMult(query7(atoi(query->args[0]), query->args[1]), i);
            free(query->args[0]);
            free(query->args[1]);
        default:
            break;
        }
        free(query->args);
        free(query);
    }
}

void freeQuery()
{
    free(list);
}

void printQueries(LinkedList *l)
{
    char **r = (char **)iterateLL(l);
    int *s = (int *)iterateLL(l);
    for (int i = 0; i < *s; i++)
    {
        printf("%s\n", r[i]);
    }
    free(r);
    free(s);
    freeLinkedList(l);
}

void executeQuery(char id, char **args)
{
    system("clear");
    switch (id)
    {
    case '1':
        printf("%s\n", query1(args[0]));
        break;
    case '2':
        printQueries(query2(atoi(args[0])));
        break;
    case '3':
        printQueries(query3(atoi(args[0])));
        break;
    case '4':
        printf("%.3lf\n", query4(args[0]));
        break;
    case '6':
        printf("%.3lf\n", query6(args[0], args[1], args[2]));
        break;
    case '7':
        printQueries(query7(atoi(args[0]), args[1]));
    default:
        break;
    }
}