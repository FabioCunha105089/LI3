#include "Queries.h"
#include "Driver.h"
#include "Ride.h"
#include "User.h"
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

double getPrice(char *car_class, double distance)
{
    if (strcmp(car_class, "green") == 0)
    {
        return 4 + distance * 0.79;
    }
    if (strcmp(car_class, "premium") == 0)
    {
        return 5.2 + distance * 0.94;
    }
    return 3.25 + distance * 0.62;
}

double query4(GHashTable *hash, ArrayList *drivers, char *city)
{
    if (g_hash_table_contains(hash, city) == FALSE)
        return 0;

    double tPrice = 0;
    char *id; // Tem de se dar free?
    int nRides;
    Ride *ride;
    LinkedList *list = (LinkedList *)g_hash_table_lookup(hash, city);
    nRides = getLLSize(list);
    for (int i = 0; i < nRides; i++)
    {
        ride = (Ride *)findByIndex(list, i);
        id = getDriver(ride);
        tPrice += getPrice(getCarClass((Driver *)find(drivers, id, findDriverArrayID)), getDistance(ride));
    }
    return tPrice / nRides;
}

double *calculateAvgScoreAndTotalPay(LinkedList *list, ArrayList *drivers, int size, char *carClass, char driverOrUser)
{
    double *res = (int *)malloc(sizeof(int) * 2);
    double score = 0, pay = 0;
    Ride *ride;
    if (driverOrUser == 'd')
    {
        for (int i = 0; i < size; i++)
        {
            ride = (Ride *)findByIndex(list, i);
            score += getScoreDriver(ride);
            pay += getPrice(carClass, getDistance(ride)) + getTip(ride);
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            ride = (Ride *)findByIndex(list, i);
            score += getScoreUser(ride);
            carClass = (char *) find(drivers, getDriver(ride), findDriverCarClass);
            pay += getPrice(carClass, getDistance(ride)) + getTip(ride);
        }
    }

    free(ride);
    res[0] = score / size;
    res[1] = pay;
    return res;
}

char *query1_drivers(char *id, GHashTable *ridesHash, ArrayList *drivers)
{
    Driver *driver = (Driver *)find(drivers, id, findDriverArrayID);
    if (isDriverActive(driver) == FALSE)
        return "";

    char aux[100];
    char *r = getDriverName(driver);
    strcat(r, ";");
    char gender = getDriverGender(driver);
    strncat(r, &gender, 1);
    strcat(r, ";");
    sprintf(aux, "%d", calculateAge(getDriverBirth(driver)));
    strcat(r, aux);
    strcat(r, ";");

    if (g_hash_table_contains(ridesHash, getDriverID(driver)) == FALSE)
    {
        strcat(r, "0.000;0;0.000");
        return r;
    }

    LinkedList *rides = (LinkedList *)g_hash_table_lookup(ridesHash, id);
    int nRides = getLLSize(rides);
    double *values = calculateAvgScoreAndTotalPay(rides, NULL, nRides, getCarClass(driver), 'd');
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

char *query1(char *id, GHashTable *driversHash, GHashTable *usersHash, ArrayList *driversList, ArrayList *userList)
{
    return atoi(id) != 0 ? query1_drivers(id, driversHash, driversList) : "wip";
}
