#include "Ride.h"
#include "stdio.h"
#include "Date.h"
#include <string.h>
#include "LinkedList.h"
#include "ArrayList.h"
#include "Driver.h"
#include "User.h"
#include <glib.h>
#include <stdlib.h>

typedef struct ride
{
    char *id;
    Date *date;
    char *driver;
    char *user;
    char *city;
    double distance;
    int score_user;
    int score_driver;
    double tip;
    char *comment;
} Ride;

static ArrayList *list = NULL;
static GHashTable *hashCity = NULL;
static GHashTable *hashDriver = NULL;
static GHashTable *hashUsers = NULL;
static GHashTable *hashAccAges = NULL;

void loadRide(char *sp)
{
    Ride *ride = (Ride *)malloc(sizeof(Ride));
    ride->id = strsep(&sp, ";");
    ride->date = sToDate(strsep(&sp, ";"));
    ride->driver = strsep(&sp, ";");
    ride->user = strsep(&sp, ";");
    ride->city = strsep(&sp, ";");
    ride->distance = atof(strsep(&sp, ";"));
    ride->score_user = atoi(strsep(&sp, ";"));
    ride->score_driver = atoi(strsep(&sp, ";"));
    ride->tip = atof(strsep(&sp, ";"));
    ride->comment = strsep(&sp, "\n");
    addAL(list, ride);
}

void initHashTables()
{
    int size = getALSize(list);
    hashCity = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeLinkedList);
    hashDriver = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeLinkedList);
    hashUsers = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeLinkedList);
    hashAccAges = g_hash_table_new_full(g_direct_equal, g_direct_equal, NULL, freeLinkedList);
    Ride *ride = NULL;
    int age;

    for (int i = 0; i < size; i++)
    {
        ride = (Ride *)getByIndex(list, i);
        // Cidades
        if (g_hash_table_contains(hashCity, ride->city) == FALSE)
        {
            LinkedList *rideList = createLL();
            addLL(rideList, ride);
            g_hash_table_insert(hashCity, ride->city, rideList);
        }
        else
        {
            addLL((LinkedList *)g_hash_table_lookup(hashCity, ride->city), ride);
        }

        // Drivers
        if (g_hash_table_contains(hashDriver, ride->driver) == FALSE)
        {
            LinkedList *rideList = createLL();
            addLL(rideList, ride);
            g_hash_table_insert(hashDriver, ride->driver, rideList);
        }
        else
        {
            addLL((LinkedList *)g_hash_table_lookup(hashDriver, ride->driver), ride);
        }

        // Users
        if (g_hash_table_contains(hashUsers, ride->user) == FALSE)
        {
            LinkedList *rideList = createLL();
            addLL(rideList, ride);
            g_hash_table_insert(hashUsers, ride->user, rideList);
        }
        else
        {
            addLL((LinkedList *)g_hash_table_lookup(hashUsers, ride->user), ride);
        }

        // Account ages
        age = getDriverAccAge(ride->driver);
        if (g_hash_table_contains(hashAccAges, GINT_TO_POINTER(age)) == FALSE)
        {
            LinkedList *rideList = createLL();
            addLL(rideList, ride);
            g_hash_table_insert(hashAccAges, GINT_TO_POINTER(age), rideList);
        }
        else
        {
            addLL((LinkedList *)g_hash_table_lookup(hashAccAges, GINT_TO_POINTER(age)), ride);
        }
    }
}

void initListRide(int size)
{
    if (!list)
        list = createAL(size - 1, sizeof(Ride *));
}

gboolean doesCityHaveRides(char *city)
{
    return g_hash_table_contains(hashCity, city);
}

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

double avgPayInCity(char *city)
{
    if (g_hash_table_contains(hashCity, city) == FALSE)
    {
        printf("Nao contem %s", city);
        return 0;
    }

    double tPrice = 0;
    Ride *ride;
    LinkedList *rideList = (LinkedList *)g_hash_table_lookup(hashCity, city);
    int nRides = getLLSize(rideList);
    for (int i = 0; i < nRides; i++)
    {
        ride = (Ride *)iterateLL(rideList);
        tPrice += getPrice(getCarClass(ride->driver), ride->distance);
    }
    return tPrice / nRides;
}

gboolean doesDriverHaveRides(char *id)
{
    return g_hash_table_contains(hashDriver, id);
}

gboolean doesUserHaveRides(char *username)
{
    return g_hash_table_contains(hashUsers, username);
}

double calculateDriverAvgScore(char *id)
{
    LinkedList *rides = (LinkedList *)g_hash_table_lookup(hashDriver, id);
    double score = 0;
    int nRides = getLLSize(rides);
    Ride *ride;
    for (int i = 0; i < nRides; i++)
    {
        ride = (Ride *)iterateLL(rides);
        score += ride->score_driver;
    }
    return score / nRides;
}

double *calculateDriverAvgScoreAndPay(char *id)
{
    LinkedList *rides = (LinkedList *)g_hash_table_lookup(hashDriver, id);
    double score = 0, pay = 0;
    int nRides = getLLSize(rides);
    Ride *ride;
    for (int i = 0; i < nRides; i++)
    {
        ride = (Ride *)iterateLL(rides);
        score += ride->score_driver;
        pay += getPrice(getCarClass(id), ride->distance) + ride->tip;
    }
    double *values = (double *)malloc(sizeof(double) * 2);
    values[0] = score / nRides;
    values[1] = pay;
    return values;
}

double calculateTotalPayDriver(char *id)
{
    LinkedList *rides = (LinkedList *)g_hash_table_lookup(hashDriver, id);
    double pay = 0;
    int nRides = getLLSize(rides);
    Ride *ride;
    for (int i = 0; i < nRides; i++)
    {
        ride = (Ride *)iterateLL(rides);
        pay += getPrice(getCarClass(id), ride->distance) + ride->tip;
    }
    return pay;
}

double calculateUserAvgScore(char *username)
{
    LinkedList *rides = (LinkedList *)g_hash_table_lookup(hashUsers, username);
    double score = 0;
    int nRides = getLLSize(rides);
    Ride *ride;
    for (int i = 0; i < nRides; i++)
    {
        ride = (Ride *)iterateLL(rides);
        score += ride->score_user;
    }
    return score / nRides;
}

double *calculateUserAvgScoreAndPay(char *username)
{
    LinkedList *rides = (LinkedList *)g_hash_table_lookup(hashUsers, username);
    double score = 0, pay = 0;
    int nRides = getLLSize(rides);
    Ride *ride;
    for (int i = 0; i < nRides; i++)
    {
        ride = (Ride *)iterateLL(rides);
        score += ride->score_user;
        pay += getPrice(getCarClass(ride->driver), ride->distance) + ride->tip;
    }
    double *values = (double *)malloc(sizeof(double) * 2);
    values[0] = score / nRides;
    values[1] = pay;
    return values;
}

double calculateTotalPayUser(char *username)
{
    LinkedList *rides = (LinkedList *)g_hash_table_lookup(hashUsers, username);
    double pay = 0;
    int nRides = getLLSize(rides);
    Ride *ride;
    for (int i = 0; i < nRides; i++)
    {
        ride = (Ride *)iterateLL(rides);
        pay += getPrice(getCarClass(ride->driver), ride->distance) + ride->tip;
    }
    return pay;
}

int getNumberOfRidesDriver(char *id)
{
    return getLLSize((LinkedList *)g_hash_table_lookup(hashDriver, id));
}

int getNumberOfRidesUser(char *username)
{
    return getLLSize((LinkedList *)g_hash_table_lookup(hashUsers, username));
}

void freeRide()
{
    freeArrayList(list);
    g_hash_table_destroy(hashCity);
    g_hash_table_destroy(hashDriver);
    g_hash_table_destroy(hashUsers);
}

void _increaseSize(char **array, int size)
{
    array = realloc(array, sizeof(char *) * size * 2);
}

LinkedList *ridesByGenderAndAge(char gender, int years)
{
    Ride *ride;
    char *d = (char *)malloc(256);
    char *u = (char *)malloc(256);
    int auxYears = years, nextPos = 0, arrSize = 1000;
    char **s = malloc(sizeof(char *) * arrSize);
    LinkedList *ridesList;
    while (1)
    {
        ridesList = (LinkedList *)g_hash_table_lookup(hashAccAges, GINT_TO_POINTER(auxYears));
        if(!ridesList)
            break;
        int size = getLLSize(ridesList);
        for (int i = 0; i < size; i++)
        {
            ride = (Ride *)iterateLL(ridesList);
            LinkedList *user = getUserGenderAccAgeName(ride->user);
            char *userGender = (char *)iterateLL(user);
            int *userAccAge = (int *)iterateLL(user);
            if (getDriverGender(ride->driver) == gender && userGender[0] == gender && *userAccAge >= auxYears)
            {
                d = getDriverName(ride->driver);
                u = iterateLL(user);
                free(user);
                s[nextPos] = (char *)malloc(strlen(d) + strlen(ride->driver) + strlen(u) + strlen(ride->user) + strlen(ride->id) + 50);
                strcpy(s[nextPos], ride->driver);
                strcat(s[nextPos], ";");
                strcat(s[nextPos], d);
                strcat(s[nextPos], ";");
                strcat(s[nextPos], ride->user);
                strcat(s[nextPos], ";");
                strcat(s[nextPos], u);
                strcat(s[nextPos], "?");
                strcat(s[nextPos], ride->id);
                if (nextPos == arrSize)
                {
                    arrSize *= 2;
                    s = realloc(s, sizeof(s) * arrSize);
                }
                nextPos++;
            }
        }
        auxYears++;
    }
    if(auxYears == years)
        s = NULL;
    int *ptr = (int *)malloc(sizeof(int));
    ptr[0] = nextPos - 1;
    LinkedList *res = createLL();
    addLL(res, ptr);
    addLL(res, s);
    return res;
}