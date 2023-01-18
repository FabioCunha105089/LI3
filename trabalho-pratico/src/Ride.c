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
    int distance;
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
int MAXYEARS = 0;

bool validateNumber(char *s, int l)
{
    if (l == 0)
        return false;
    for (int i = 0; i < l; i++)
    {
        if (isdigit(s[i]) == 0 && s[i] != '.')
            return false;
    }
    return true;
}

bool validateNumberInt(char *s, int l)
{
    if (l == 0)
        return false;
    for (int i = 0; i < l; i++)
    {
        if (isdigit(s[i]) == 0)
            return false;
    }
    return true;
}

int loadRide(char *sp)
{
    Ride *ride = (Ride *)malloc(sizeof(Ride));
    ride->id = strdup(strsep(&sp, ";"));
    char *aux1, *aux2;
    if (strlen(ride->id) == 0)
    {
        free(ride->id);
        free(ride);
        return 0;
    }
    aux1 = strdup(strsep(&sp, ";"));
    ride->date = sToDate(aux1, strlen(aux1));
    if (!ride->date)
    {
        free(ride->id);
        free(ride);
        free(aux1);
        return 0;
    }
    free(aux1);
    ride->driver = strdup(strsep(&sp, ";"));
    if (strlen(ride->driver) == 0)
    {
        free(ride->id);
        free(ride->date);
        free(ride->driver);
        free(ride);
        return 0;
    }
    ride->user = strdup(strsep(&sp, ";"));
    if (strlen(ride->user) == 0)
    {
        free(ride->id);
        free(ride->date);
        free(ride->driver);
        free(ride->user);
        free(ride);
        return 0;
    }
    ride->city = strdup(strsep(&sp, ";"));
    if (strlen(ride->city) == 0)
    {
        free(ride->id);
        free(ride->date);
        free(ride->driver);
        free(ride->user);
        free(ride->city);
        free(ride);
        return 0;
    }
    aux1 = strdup(strsep(&sp, ";"));
    if (validateNumberInt(aux1, strlen(aux1)) == false || atoi(aux1) <= 0)
    {
        free(aux1);
        free(ride->id);
        free(ride->date);
        free(ride->driver);
        free(ride->user);
        free(ride->city);
        free(ride);
        return 0;
    }
    ride->distance = atoi(aux1);
    free(aux1);
    aux1 = strdup(strsep(&sp, ";"));
    aux2 = strdup(strsep(&sp, ";"));
    if (validateNumberInt(aux1, strlen(aux1)) == false || validateNumberInt(aux2, strlen(aux2)) == false || atof(aux1) < 1 || atof(aux2) < 1)
    {
        free(aux1);
        free(aux2);
        free(ride->id);
        free(ride->date);
        free(ride->driver);
        free(ride->user);
        free(ride->city);
        free(ride);
        return 0;
    }
    ride->score_user = atof(aux1);
    ride->score_driver = atof(aux2);
    free(aux1);
    free(aux2);
    aux1 = strdup(strsep(&sp, ";"));
    if (validateNumber(aux1, strlen(aux1)) == false)
    {
        free(aux1);
        free(ride->id);
        free(ride->date);
        free(ride->driver);
        free(ride->user);
        free(ride->city);
        free(ride);
        return 0;
    }
    ride->tip = atof(aux1);
    free(aux1);
    ride->comment = strdup(strsep(&sp, "\n"));
    addAL(list, ride);
    return 1;
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

int compareRidesByDate(const void *A, const void *B) 
{
    Ride *a = *(Ride **)A;
    Ride *b = *(Ride **)B;
 
    return isDateBigger(a->date, b->date);
}

void initHashTables()
{
    int size = getALSize(list), ageD, ageU;
    char genderD, genderU;
    hashCity = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeLinkedList);
    hashDriver = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeLinkedList);
    hashUsers = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeLinkedList);
    hashAccAges = g_hash_table_new_full(g_direct_equal, g_direct_equal, NULL, freeLinkedList);
    hashDriverCityScores = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    Ride *ride = NULL;
    quickSortArrayList(list, sizeof(Ride *), compareRidesByDate);

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

        // AccAges
        if (isDriverActive(ride->driver) == true && isUserActive(ride->user) == TRUE)
        {
            ageD = getDriverAccAge(ride->driver);
            ageU = getUserAccAge(ride->user);
            genderD = getDriverGender(ride->driver);
            genderU = getUserGender(ride->user);
            if (ageD > MAXYEARS)
                MAXYEARS = ageD;
            if (ageU > MAXYEARS)
                MAXYEARS = ageU;
            if (ageD == ageU && genderD == genderU)
            {
                int *age = (int *)malloc(sizeof(int));
                *age = ageD * genderD;
                if (g_hash_table_contains(hashAccAges, *age) == FALSE)
                {
                    LinkedList *rideList = createLL();
                    addLL(rideList, ride);
                    g_hash_table_insert(hashAccAges, *age, rideList);
                }
                else
                {
                    addLL((LinkedList *)g_hash_table_lookup(hashAccAges, *age), ride);
                }
            }
        }

        // Driver score by city
        char *driverScore = (char *)malloc(256);
        strcpy(driverScore, ride->driver);
        strcat(driverScore, ride->city);
        double *score_Nrides = (double *)malloc(sizeof(double) * 2);
        score_Nrides[0] = ride->score_driver;
        if (g_hash_table_contains(hashDriverCityScores, driverScore) == FALSE)
        {
            score_Nrides[1] = 1;
            g_hash_table_insert(hashDriverCityScores, driverScore, score_Nrides);
        }
        else
        {
            double *temp = (double *)g_hash_table_lookup(hashDriverCityScores, driverScore);
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
    if (!rides || nRides == 0)
    {
        return 0;
    }
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

void _freeRide(void *r)
{
    Ride *ride = (Ride *)r;
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

double avgPayByDate(char *date1, char *date2)
{
    Date *dateA = sToDateSimple(date1);
    Date *dateB = sToDateSimple(date2);
    double tPay = 0;
    Ride *ride;
    int tRides = getALSize(list), nRides = 0;
    int checkDate = false;

    for (int i = 0; i < tRides; i++)
    {
        ride = (Ride *)getByIndex(list, i);

        if(isDateBigger(ride->date, dateA) >= 0 && isDateBigger(dateB, ride->date) >= 0)
        {
            checkDate = true;
            nRides++;
            tPay += getPrice(getCarClass(ride->driver), ride->distance);

        } else if (checkDate) break;
    }

    free(dateA);
    free(dateB);
    if(nRides==0) return 0;

    return tPay / nRides; 
}

double avgDistanceInCityByDate(char *city, char *date1, char *date2)
{

    if (g_hash_table_contains(hashCity, city) == FALSE)
    {

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

    for (int i = 0; i < tRides; i++)
    {

        ride = (Ride *)iterateLL(rideList);

        if (isDateBigger(ride->date, dateA) >= 0 && isDateBigger(dateB, ride->date) >= 0)
        {
            tDistance += ride->distance;
            nRides++;
        }
    }
    free(dateA);
    free(dateB);
    if (nRides == 0)
        return 0;

    return tDistance / nRides;
}

int compareRidesByDriverScore(const void *A, const void *B)
{
    Ride *a = *(Ride **)A;
    Ride *b = *(Ride **)B;
    char drivA[20];
    char drivB[20];
    strcpy(drivA, a->driver);
    strcat(drivA, a->city);
    strcpy(drivB, b->driver);
    strcat(drivB, b->city);
    double *score_nRidesA = (double *)g_hash_table_lookup(hashDriverCityScores, drivA);
    double *score_nRidesB = (double *)g_hash_table_lookup(hashDriverCityScores, drivB);
    double scoreA = score_nRidesA[0] / score_nRidesA[1];
    double scoreB = score_nRidesB[0] / score_nRidesB[1];
    if (scoreA < scoreB || (scoreA == scoreB && atoi(a->driver) < atoi(b->driver)))
        return -1;
    return 1;
}

char **driversByScoreInCity(char *city, int n)
{
    if (g_hash_table_contains(hashCity, city) == FALSE)
    {
        printf("Nao contem %s", city);
        return NULL;
    }
    ArrayList *rideList = LLtoAL((LinkedList *)g_hash_table_lookup(hashCity, city), sizeof(Ride *));
    quickSortArrayList(rideList, sizeof(Ride *), compareRidesByDriverScore);
    char **r = malloc(sizeof(char *) * n);
    char *id, *name;
    char aux[10];
    double score;
    int size = getALSize(rideList) - 1, pos, counter = n;
    Ride *ride;
    for (int i = size; counter > 0; counter--)
    {
        ride = (Ride *)getByIndex(rideList, i);
        pos = (counter - n) * -1;
        id = getDriverIDFromRide(ride);
        if (isDriverActive(id) == true)
        {
            name = getDriverNameFromRide(ride);
            score = getDriverAvgScoreInCityFromRide(ride);
            sprintf(aux, "%.3lf", score);
            r[pos] = (char *)malloc(strlen(id) + strlen(name) + 15);
            strcpy(r[pos], id);
            strcat(r[pos], ";");
            strcat(r[pos], name);
            strcat(r[pos], ";");
            strcat(r[pos], aux);
            for (int j = 0; j < pos; j++)
            {
                if (strcmp(r[j], r[pos]) == 0)
                {
                    free(r[pos]);
                    counter++;
                    break;
                }
            }
        }
        else
        {
            counter++;
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
    double *score = (double *)g_hash_table_lookup(hashDriverCityScores, driver);
    return score[0] / score[1];
}

Date *getMostRecentDate(char *id)
{
    int aux = atoi(id);
    Date *recent = NULL;
    if (aux != 0)
    {
        recent = getDriverRecentDate(id);
    }
    else
    {
        recent = getUserRecentDate(id);
    }

    if (recent)
        return recent;

    LinkedList *rides;
    if (aux != 0)
    {
        rides = (LinkedList *)g_hash_table_lookup(hashDriver, id);
    }
    else
    {
        rides = (LinkedList *)g_hash_table_lookup(hashUsers, id);
    }

    if (!rides)
        return NULL;

    int nRides = getLLSize(rides);
    Ride *ride = (Ride *)iterateLL(rides);
    recent = ride->date;
    for (int i = 1; i < nRides; i++)
    {
        ride = (Ride *)iterateLL(rides);
        if (isDateBigger(recent, ride->date) == -1)
            recent = ride->date;
    }
    if (aux != 0)
    {
        setDriverRecentDate(id, recent);
    }
    else
    {
        setUserRecentDate(id, recent);
    }
    return recent;
}

int mostRecentRide(char *a, char *b)
{
    int aux;
    Date *aRecent = getMostRecentDate(a);
    Date *bRecent = getMostRecentDate(b);
    if (!aRecent && bRecent)
        return 1;
    if (!bRecent && aRecent)
        return -1;
    if (!aRecent && !bRecent)
    {
        aux = 0;
    }
    else
    {
        aux = isDateBigger(bRecent, aRecent);
    }
    if (aux == 0)
    {
        if (atoi(a) != 0)
            return atoi(a) > atoi(b) ? 1 : -1;
        return strcmp(a, b) < 0 ? -1 : 1;
    }
    return aux;
}

int calculateUserTotalDist(char *username)
{
    if (g_hash_table_contains(hashUsers, username) == FALSE)
        return 0;
    LinkedList *rideList = (LinkedList *)g_hash_table_lookup(hashUsers, username);
    Ride *ride;
    int size = getLLSize(rideList);
    int tDist = 0;
    for (int i = 0; i < size; i++)
    {
        ride = (Ride *)iterateLL(rideList);
        tDist += ride->distance;
    }
    return tDist;
}

void updateRide(int newSize)
{
    updateArrayList(list, sizeof(Ride *), newSize);
}

int compareRidesByAccAge(const void *A, const void *B)
{
    Ride *a = *(Ride **)A;
    Ride *b = *(Ride **)B;

    int driverComp = isDateBigger(getDriverAccCreation(a->driver), getDriverAccCreation(b->driver));
    if (driverComp == 1)
        return 1;
    if (driverComp == 0)
    {
        int userComp = isDateBigger(getUserAccCreation(a->user), getUserAccCreation(b->user));
        if (userComp == 1)
            return 1;
        if (userComp == 0)
            return atoi(a->id) > atoi(b->id) ? 1 : -1;
    }
    return -1;
}

int compareRidesByDistance(const void *A, const void *B) 
{
    Ride *a = *(Ride **)A;
    Ride *b = *(Ride **)B;

    if (a->distance > b->distance) {
        return -1;
    } else if (a->distance < b->distance) {
        return 1;
    }
    
    int checkDate = isDateBigger(a->date, b->date) * (-1);
    if (checkDate != 0) return checkDate;
    
    return (atoi(a->id) > atoi(b->id)) ? -1 : 1;
    
}

LinkedList *ridesWithTipByDistance(char *date1, char *date2)
{
    Date *dateA = sToDateSimple(date1);
    Date *dateB = sToDateSimple(date2);
    int tSize = getALSize(list), rSize = 0, lSize = 0;
    Ride *ride;
    char **r = NULL;
    LinkedList *rideList = createLL();
    bool checkDate = false;

    if (!(isDateValid(dateA)) || !(isDateValid(dateB))) return NULL; 
    
    for (int i = 0; i < tSize; i++)
    {
        ride = (Ride *)getByIndex(list, i);

        if(isDateBigger(ride->date, dateA) >= 0 && isDateBigger(dateB, ride->date) >= 0)
        {
            checkDate = true;
            addLL(rideList, ride);
        } else if (checkDate) break;
    }
    
    lSize = getLLSize(rideList);
    free(dateA);
    free(dateB);
    if (lSize == 0) return NULL;
    ArrayList *allRides = LLtoAL(rideList, lSize);
    quickSortArrayList(allRides, sizeof(Ride *), compareRidesByDistance);
    r = (char **)malloc(sizeof(char *) * lSize);
    char aux[10];

    for (int i = 0; i < lSize; i++)
    {
        ride = (Ride *)getByIndex(allRides, i);
        r[i] = (char *)malloc(256);
        strcpy(r[i], ride->id);
        strcat(r[i], ";");
        strcat(r[i], dateToS(ride->date));
        strcat(r[i], ";");
        sprintf(aux, "%d", ride->distance);
        strcat(r[i], aux);
        strcat(r[i], ";");
        strcat(r[i], ride->city);
        strcat(r[i], ";");
        sprintf(aux, "%.3lf", ride->tip);
        strcat(r[i], aux);
    }
    
    LinkedList *l = createLL();
    addLL(l, r);
    int *s = (int *)malloc(sizeof(int));
    *s = lSize;
    addLL(l, s);
    return l;
}

LinkedList *ridesWithSameGenderAndAccAge(char gender, int years)
{
    int *age = (int *)malloc(sizeof(int));
    *age = gender * years;

    LinkedList *rideList, *fullList = createLL();
    char **r = NULL;
    int size, tSize = 0, nLists = 0;
    Ride *ride;
    while (years <= MAXYEARS)
    {
        if (g_hash_table_contains(hashAccAges, *age) == TRUE)
        {
            rideList = (LinkedList *)g_hash_table_lookup(hashAccAges, *age);
            tSize += getLLSize(rideList);
            addLL(fullList, rideList);
            nLists++;
        }
        years++;
        free(age);
        age = (int *)malloc(sizeof(int));
        *age = gender * years;
    }
    if (nLists == 0)
        return NULL;
    ArrayList *allRides = LLtoAL(iterateLL(fullList), tSize);
    for (int i = 1; i < nLists; i++)
    {
        rideList = iterateLL(fullList);
        size = getLLSize(rideList);
        for (int j = 0; j < size; j++)
        {
            addAL(allRides, iterateLL(rideList));
        }
    }
    quickSortArrayList(allRides, sizeof(Ride *), compareRidesByAccAge);
    r = (char **)malloc(sizeof(char *) * tSize);
    for (int i = 0; i < tSize; i++)
    {
        ride = (Ride *)getByIndex(allRides, i);
        r[i] = (char *)malloc(256);
        strcpy(r[i], getDriverIDAndName(ride->driver));
        strcat(r[i], ";");
        strcat(r[i], getUserUsernameAndName(ride->user));
    }
    free(age);
    LinkedList *l = createLL();
    addLL(l, r);
    int *s = (int *)malloc(sizeof(int));
    *s = tSize;
    addLL(l, s);
    return l;
}