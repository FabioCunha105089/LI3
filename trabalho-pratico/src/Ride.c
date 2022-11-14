#include "../include/Ride.h"
#include "stdio.h"
#include "../include/Date.h"
#include <string.h>
#include "../include/LinkedList.h"
#include "../include/ArrayList.h"
#include "../include/Driver.h"

typedef struct ride
{
    char *id;
    Date date;
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

int getElementSizeRide()
{
    return sizeof(Ride *);
}

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
    addLL(list, ride);
}

void initHashTables()
{
    int size = getALSize(list);
    hashCity = g_hash_table_new(g_str_hash, g_str_equal);
    hashDriver = g_hash_table_new(g_str_hash, g_str_equal);
    hashUsers = g_hash_table_new(g_str_hash, g_str_equal);
    LinkedList *listHash = createLL();
    Ride *ride = NULL;

    for (int i = 0; i < size; i++)
    {
        ride = (Ride *) getByIndex(list, i);
        // Cidades
        if (g_hash_table_contains(hashCity, ride->city) == FALSE)
        {
            LinkedList *list = createLL();
            addLL(list, ride);
            g_hash_table_insert(hashCity, ride->city, list);
        }
        else
        {
            addLL((LinkedList *)g_hash_table_lookup(hashCity, ride->city), ride);
        }

        // Drivers
        if (g_hash_table_contains(hashDriver, ride->driver) == FALSE)
        {
            LinkedList *list = createLL();
            addLL(list, ride);
            g_hash_table_insert(hashDriver, ride->driver, list);
        }
        else
        {
            addLL((LinkedList *)g_hash_table_lookup(hashDriver, ride->driver), ride);
        }

        // Users
        if (g_hash_table_contains(hashUsers, ride->user) == FALSE)
        {
            LinkedList *list = createLL();
            addLL(list, ride);
            g_hash_table_insert(hashUsers, ride->user, list);
        }
        else
        {
            addLL((LinkedList *)g_hash_table_lookup(hashUsers, ride->user), ride);
        }
    }
}

void initRide(int size){
    if(!list)
        list = createAL(size, sizeof(Ride *));
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

double avgPayInCity(char* city){
    if (g_hash_table_contains(hashCity, city) == FALSE)
        return 0;
    double tPrice = 0;
    int nRides, getLLSize(list);
    Ride *ride;
    LinkedList *list = (LinkedList *)g_hash_table_lookup(hashCity, city);
    for (int i = 0; i < nRides; i++)
    {
        ride = (Ride *)findByIndex(list, i);
        tPrice += getPrice(getCarClass(findDriverByID(ride->driver)), ride->distance);
    }
    return tPrice / nRides;
}

gboolean doesDriverHaveRides(char *id){
    return g_hash_table_contains(hashDriver, id);
}

double calculateDriverAvgScore(char *id){
    LinkedList *rides = (LinkedList *) g_hash_table_lookup(hashDriver, id);
    double score = 0;
    int nRides = getLLSize(list);
    Ride *ride;
    for(int i = 0; i < nRides; i++){
        ride = findByIndex(list, i);
        score += ride->score_driver;
    }
    return score / nRides;
}

double* calculateDriverAvgScoreAndPay(char *id){
    LinkedList *rides = (LinkedList *) g_hash_table_lookup(hashDriver, id);
    double score = 0, pay = 0;
    int nRides = getLLSize(list);
    Ride *ride;
    for(int i = 0; i < nRides; i++){
        ride = findByIndex(list, i);
        score += ride->score_driver;
        pay += getPrice(findDriverByID(id), ride->distance)+ ride->tip;
    }
    double *values = (double *) malloc (sizeof(double) * 2);
    values[0] = score / nRides;
    values[1] = pay;
    return values;
}

double calculateTotalPay(char *id){
    LinkedList *rides = (LinkedList *) g_hash_table_lookup(hashDriver, id);
    double pay = 0;
    int nRides = getLLSize(list);
    Ride *ride;
    for(int i = 0; i < nRides; i++){
        ride = findByIndex(list, i);
        pay += getPrice(findDriverByID(id), ride->distance)+ ride->tip;
    }
    return pay;
}

int getNumberOfRidesDriver(char *id)
{
    return getLLSize((LinkedList *) g_hash_table_lookup(hashDriver, id));
}
