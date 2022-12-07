#include "Ride.h"
#include <string.h>
#include "LinkedList.h"
#include "ArrayList.h"
#include "Driver.h"
#include "User.h"
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

typedef struct ride
{
    char *id;
    Date *date;
    char *driver;
    char *user;
    char *city;
    double distance;
    double score_user;
    double score_driver;
    double tip;
    char *comment;
} Ride;

static ArrayList *list = NULL;
static GHashTable *hashCity = NULL;
static GHashTable *hashDriver = NULL;
static GHashTable *hashUsers = NULL;
static GHashTable *hashAccAges = NULL;
static GHashTable *hashDriverCityScores = NULL;

bool validateScore(char * s, int l)
{
    for(int i = 0; i < l; i++)
    {
        if(isdigit(s[i]) == 0 && s[i] != '.')
            return false;
    }
    return true;
}

void loadRide(char *sp)
{
    Ride *ride = (Ride *)malloc(sizeof(Ride));
    ride->id = strdup(strsep(&sp, ";"));
    char *aux1, *aux2;
    if(strlen(ride->id) == 0)
    {
        free(ride->id);
        free(ride);
        return;
    }
    aux1 = strdup(strsep(&sp, ";"));
    ride->date = sToDate(aux1, strlen(aux1));
    if(!ride->date)
    {
        free(ride->id);
        free(ride);
        free(aux1);
        return;
    }
    free(aux1);
    ride->driver = strdup(strsep(&sp, ";"));
    if(strlen(ride->driver) == 0)
    {
        free(ride->id);
        free(ride->date);
        free(ride->driver);
        free(ride);
        return;
    }
    ride->user = strdup(strsep(&sp, ";"));
    if(strlen(ride->user) == 0)
    {
        free(ride->id);
        free(ride->date);
        free(ride->driver);
        free(ride->user);
        free(ride);
        return;
    }
    ride->city = strdup(strsep(&sp, ";"));
    if(strlen(ride->city) == 0)
    {
        free(ride->id);
        free(ride->date);
        free(ride->driver);
        free(ride->user);
        free(ride->city);
        free(ride);
        return;
    }
    ride->distance = atof(strsep(&sp, ";"));
    if(ride->distance < 0)
    {
        free(ride->id);
        free(ride->date);
        free(ride->driver);
        free(ride->user);
        free(ride->city);
        free(ride);
        return;
    }
    aux1 = strdup(strsep(&sp, ";"));
    aux2 = strdup(strsep(&sp, ";"));
    if(validateScore(aux1, strlen(aux1)) == false || validateScore(aux2, strlen(aux2)) == false)
    {
        free(aux1);
        free(aux2);
        free(ride->id);
        free(ride->date);
        free(ride->driver);
        free(ride->user);
        free(ride->city);
        free(ride);
        return;
    }
    ride->score_user = atof(aux1);
    ride->score_driver = atof(aux2);
    free(aux1);
    free(aux2);
    ride->tip = atof(strsep(&sp, ";"));
    ride->comment = strdup(strsep(&sp, "\n"));
    addAL(list, ride);
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

void initHashTables()
{
    int size = getALSize(list);
    hashCity = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeLinkedList);
    hashDriver = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeLinkedList);
    hashUsers = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeLinkedList);
    hashAccAges = g_hash_table_new_full(g_direct_equal, g_direct_equal, NULL, freeLinkedList);
    hashDriverCityScores = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    Ride *ride = NULL;

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

        //Driver score by city
        char *driverScore = (char *) malloc(256);
        strcpy(driverScore, ride->driver);
        strcat(driverScore, ride->city);
        double *score_Nrides = (double *) malloc(sizeof(double) * 2);
        score_Nrides[0] = ride->score_driver;
        if (g_hash_table_contains(hashDriverCityScores, driverScore) == FALSE)
        {
            score_Nrides[1] = 1;
            g_hash_table_insert(hashDriverCityScores, driverScore, score_Nrides);
        }
        else
        {
            double *temp = (double *) g_hash_table_lookup(hashDriverCityScores, driverScore);
            score_Nrides[0] += temp[0];
            score_Nrides[1] = temp[1] + 1;
            g_hash_table_replace(hashDriverCityScores, driverScore, score_Nrides);
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

void _freeRide(void * r)
{
    Ride *ride = (Ride *) r;
    free(ride->date);
    free(ride->id);
    free(ride->user);
    free(ride->driver);
    free(ride->city);
    free(ride->comment);
    free(ride);
}

void freeRide()
{
    freeArrayList(list, _freeRide);
    g_hash_table_destroy(hashCity);
    g_hash_table_destroy(hashDriver);
    g_hash_table_destroy(hashUsers);
    g_hash_table_destroy(hashDriverCityScores);
    g_hash_table_destroy(hashAccAges);
}

double avgDistanceInCityByDate (char *city, char *date1, char *date2) {

    if (g_hash_table_contains(hashCity, city) == FALSE) {

        printf("Nao contem %s", city);
        return 0;
    }

    Date *dateA = sToDateSimple(date1);
    Date *dateB = sToDateSimple(date2);
    double tDistance = 0;
    Ride *ride;
    LinkedList *rideList = (LinkedList *)g_hash_table_lookup(hashCity, city);
    int tRides = getLLSize(rideList);
    int nRides = 0;

    for (int i = 0; i < tRides; i++) {

        ride = (Ride *)iterateLL(rideList);

        if (isDateBigger(ride->date, dateA) >=0 && isDateBigger(dateB, ride->date) >= 0) {
            
            tDistance += ride->distance;
            nRides++;
        }
    }

    return tDistance/nRides;

}

int compareRidesByDriverScore(const void *A, const void *B)
{
    Ride *a = *(Ride **) A;
    Ride *b = *(Ride **) B;
    char drivA[20];
    char drivB[20];
    strcpy(drivA, a->driver);
    strcat(drivA, a->city);
    strcpy(drivB, b->driver);
    strcat(drivB, b->city);
    double *score_nRidesA = (double *) g_hash_table_lookup(hashDriverCityScores, drivA);
    double *score_nRidesB = (double *) g_hash_table_lookup(hashDriverCityScores, drivB);
    double scoreA = score_nRidesA[0] / score_nRidesA[1];
    double scoreB = score_nRidesB[0] / score_nRidesB[1];
    if(scoreA < scoreB || (scoreA == scoreB && atoi(a->driver) < atoi(b->driver)))
        return -1;
    return 1;
}

char **driversByScoreInCity(char *city, int n)
{
    ArrayList *rideList = LLtoAL((LinkedList *)g_hash_table_lookup(hashCity, city), sizeof(Ride *));
    quickSortArrayList(rideList, sizeof(Ride *), compareRidesByDriverScore);
    char **r = malloc(sizeof(char *) * n);
    char *id, *name;
    char aux[10];
    double score;
    int size = getALSize(rideList) -1 , pos, counter = n;
    for(int i = size; counter > 0; counter--)
    {
        pos = (counter - n) * -1;
        id = getDriverIDFromRide((Ride *) getByIndex(rideList, i));
        name = getDriverNameFromRide((Ride *) getByIndex(rideList, i));
        score = getDriverAvgScoreInCityFromRide((Ride *) getByIndex(rideList, i));
        sprintf(aux, "%.3f", score);
        r[pos] = (char *) malloc(strlen(id) + strlen(name) + 15);
        strcpy(r[pos], id);
        strcat(r[pos], ";");
        strcat(r[pos], name);
        strcat(r[pos], ";");
        strcat(r[pos], aux);
        for(int j = 0; j < pos; j++)
        {
            if(strcmp(r[j], r[pos]) == 0)
            {
                free(r[pos]);
                counter++;
                break;
            }
        }
        i--;
    }
    freeArrayListSimple(rideList);
    return r;
}

char *getDriverIDFromRide(Ride *ride)
{
    return ride->driver;
}

char *getDriverNameFromRide(Ride *ride)
{
    return getDriverName(ride->driver);
}

double getDriverAvgScoreInCityFromRide(Ride *ride)
{
    char driver[20];
    strcpy(driver, ride->driver);
    strcat(driver, ride->city);
    double *score = (double *) g_hash_table_lookup(hashDriverCityScores, driver);
    return score[0] / score[1];
}

int mostRecentRide(char *a, char *b)
{
    LinkedList *aRides = (LinkedList *) g_hash_table_lookup(hashDriver, a);
    LinkedList *bRides = (LinkedList *) g_hash_table_lookup(hashDriver, b);
    int aNrides = getLLSize(aRides), bNrides = getLLSize(bRides);
    Date *defaultDate = setDefaultDate();
    Date *aRecent = defaultDate;
    Date *bRecent = aRecent;
    Ride *ride;
    for(int i = 0; i < aNrides; i++)
    {
        ride = (Ride *) iterateLL(aRides);
        if(isDateBigger(aRecent, ride->date) == 0)
            aRecent = ride->date;
    }

    for(int i = 0; i < bNrides; i++)
    {
        ride = (Ride *) iterateLL(bRides);
        if(isDateBigger(bRecent, ride->date) == 0)
            bRecent = ride->date;
    }

    int aux = isDateBigger(aRecent, bRecent);
    free(defaultDate);
    if(aux == 0)
        return atoi(a) > atoi(b) ? -1 : 1;
    return aux;
}