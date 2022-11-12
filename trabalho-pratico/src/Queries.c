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
    query->id = strsep(&sp, " ");
    int nArgs = getNArgs(query->id);
    query->args = (char **) malloc(sizeof(char*) * nArgs);
    for(int i = 0; i < nArgs; i++)
    {
        query->args[i] = strsep(&sp, " ");
    }
    addLL(list, query);
}

double query4(char *city)
{
    return avgPayInCity(city);
}

// double *calculateAvgScoreAndTotalPay(LinkedList *list, ArrayList *drivers, int size, char *carClass, char driverOrUser)
// {
//     double *res = (int *)malloc(sizeof(int) * 2);
//     double score = 0, pay = 0;
//     Ride *ride;
//     if (driverOrUser == 'd')
//     {
//         for (int i = 0; i < size; i++)
//         {
//             ride = (Ride *)findByIndex(list, i);
//             score += getScoreDriver(ride);
//             pay += getPrice(carClass, getDistance(ride)) + getTip(ride);
//         }
//     }
//     else
//     {
//         for (int i = 0; i < size; i++)
//         {
//             ride = (Ride *)findByIndex(list, i);
//             score += getScoreUser(ride);
//             carClass = (char *) find(drivers, getDriver(ride), findDriverCarClass);
//             pay += getPrice(carClass, getDistance(ride)) + getTip(ride);
//         }
//     }

//     res[0] = score / size;
//     res[1] = pay;
//     return res;
// }

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

char *query1_users(char *username, GHashTable *ridesHash, ArrayList *users, ArrayList* drivers)
{
    User *user = (User *)find(users, username, findUserArrayID);
    if (isUserActive(user) == FALSE)
        return "";
    char aux[100];
    char *r = getUserName(user);
    strcat(r, ";");
    char gender = getUserGender(user);
    strncat(r, &gender, 1);
    strcat(r, ";");
    sprintf(aux, "%d", calculateAge(getUserBirth(user)));
    strcat(r, aux);
    strcat(r, ";");

    if (g_hash_table_contains(ridesHash, getUserUsername(user)) == FALSE)
    {
        strcat(r, "0.000;0;0.000");
        return r;
    }

    LinkedList *rides = (LinkedList *)g_hash_table_lookup(ridesHash, username);
    int nRides = getLLSize(rides);
    double *values = calculateAvgScoreAndTotalPay(rides, drivers, nRides, "", 'u');
    double score = values[0];
    double pay = values[1];
    sprintf(aux, "%.3f", score);
    strcat(r, aux);
    strcat(r, ";");
    sprintf(aux, "%d", nRides);
    strcat(r, aux);
    strcat(r, ";");
    sprintf(aux, "%.3f", pay);
    strcat(r, aux);
    free(values);
    return r;
}

char *query1(char *id)
{
    //return atoi(id) != 0 ? query1_drivers(id) : query1_users(id, usersHash, userList, driversList);
    return atoi(id) != 0 ? query1_drivers(id) : "f";
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