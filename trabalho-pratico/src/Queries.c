#include "Queries.h"
#include "Driver.h"
#include "Ride.h"
#include "User.h"
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

typedef struct query{
    char id;
    char **args;
}Query;

static ArrayList *list;

void initListQuery(int size)
{
    if(!list)
        list = createAL(size, sizeof(Query *));
}

int getNArgs(char id)
{
    switch (id)
    {
        case '1':
        case '4':
            return 1;
            break;

        case '6':
            return 3;
            break; 
    }
}

void loadQuery(char *sp)
{
    Query *query = (Query *) malloc(sizeof(Query));
    query->id = strsep(&sp, " ")[0];
    int nArgs = getNArgs(query->id);
    query->args = (char **) malloc(sizeof(char*) * nArgs);
    for(int i = 0; i < nArgs; i++)
    {
        query->args[i] = strsep(&sp, " ");
    }
    addAL(list, query);
}

double query4(char *city)
{
    return avgPayInCity(city);
}

char *query1_drivers(char *id)
{
    if(isDriverActive(id) == FALSE)
        return "";

    char *r = getDriverBasicInfo(id);

    if(doesDriverHaveRides(id) == FALSE){
        strcat(r, "0.000;0;0.000");
        return r;
    }

    double * values = getDriverAvgScoreAndPay(id);
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

char *query1_users(char *id) {
    
    if(isUserActive(id) == FALSE)
        return "";

    char *r = getUserBasicInfo(id);

    if(doesUserHaveRides(id) == FALSE){
        strcat(r, "0,000;0;0.000");
        return r;
    }

    double * values = getUserAvgScoreAndPay(id);
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
    for(int i = 0; i < nQueries; i++)
    {
        Query *query = (Query *) getByIndex(list, i);
        switch(query->id)
        {
            case '1':
                query1(query->args[0]);
                break;

            case '4':
                query4(query->args[0]);
                break;

            case '6':
                //query6(query->args[0], sToDate(query->args[1]), sToDate(query->args[2]));
                break;
        }
    }
}