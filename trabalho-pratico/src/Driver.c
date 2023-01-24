#include "Driver.h"
#include <string.h>
#include <stdlib.h>
#include "ArrayList.h"
#include "Ride.h"
#include <ctype.h>

typedef struct driver
{
    char *id;
    char *name;
    Date *birth_day;
    char gender;
    char *car_class;
    char *city;
    Date *account_creation;
    bool account_status;
    int age;
    int account_age;
    double avgScore;
    Date *recentRide;
    double *scorePerCity;
} Driver;

static ArrayList *list = NULL;

void initListDriver(int size)
{
    if (!list)
        list = createAL(size - 1, sizeof(Driver *));
}

int loadDriver(char *sp)
{
    Driver *driver = (Driver *)malloc(sizeof(Driver));
    char *aux;
    driver->id = strdup(strsep(&sp, ";"));
    if (strlen(driver->id) == 0)
    {
        free(driver->id);
        free(driver);
        addAL(list, NULL);
        return 0;
    }
    driver->name = strdup(strsep(&sp, ";"));
    if (strlen(driver->name) == 0)
    {
        free(driver->id);
        free(driver->name);
        free(driver);
        addAL(list, NULL);
        return 0;
    }
    aux = strdup(strsep(&sp, ";"));
    driver->birth_day = sToDate(aux, strlen(aux));
    if (!driver->birth_day)
    {
        free(driver->id);
        free(driver->name);
        free(driver);
        free(aux);
        addAL(list, NULL);
        return 0;
    }
    free(aux);
    driver->gender = strsep(&sp, ";")[0];
    if (driver->gender == '\0')
    {
        free(driver->id);
        free(driver->name);
        free(driver->birth_day);
        free(driver);
        addAL(list, NULL);
        return 0;
    }
    driver->car_class = strdup(strsep(&sp, ";"));
    for (int i = 0; i < strlen(driver->car_class); i++)
    {
        driver->car_class[i] = tolower(driver->car_class[i]);
    }
    if (strcmp(driver->car_class, "premium") != 0 && strcmp(driver->car_class, "basic") != 0 && strcmp(driver->car_class, "green") != 0)
    {
        free(driver->id);
        free(driver->name);
        free(driver->birth_day);
        free(driver->car_class);
        free(driver);
        addAL(list, NULL);
        return 0;
    }
    aux = strdup(strsep(&sp, ";"));
    if (strlen(aux) == 0)
    {
        free(driver->id);
        free(driver->name);
        free(driver->birth_day);
        free(driver->car_class);
        free(aux);
        free(driver);
        addAL(list, NULL);
        return 0;
    }
    free(aux);
    driver->city = strdup(strsep(&sp, ";"));
    if (strlen(driver->city) == 0)
    {
        free(driver->id);
        free(driver->name);
        free(driver->birth_day);
        free(driver->car_class);
        free(driver->city);
        free(driver);
        addAL(list, NULL);
        return 0;
    }
    aux = strdup(strsep(&sp, ";"));
    driver->account_creation = sToDate(aux, strlen(aux));
    if (!driver->account_creation)
    {
        free(driver->id);
        free(driver->name);
        free(driver->birth_day);
        free(driver->car_class);
        free(driver->city);
        free(driver->account_creation);
        free(driver);
        free(aux);
        addAL(list, NULL);
        return 0;
    }
    free(aux);
    aux = strdup(strsep(&sp, "\n"));
    for (int i = 0; i < strlen(aux); i++)
    {
        aux[i] = tolower(aux[i]);
    }
    if (strcmp(aux, "active") == 0)
    {
        driver->account_status = true;
    }
    else if (strcmp(aux, "inactive") == 0)
    {
        driver->account_status = false;
    }
    else
    {
        free(aux);
        free(driver->id);
        free(driver->name);
        free(driver->birth_day);
        free(driver->car_class);
        free(driver->city);
        free(driver->account_creation);
        free(driver);
        addAL(list, NULL);
        return 0;
    }
    driver->age = calculateAge(driver->birth_day);
    driver->account_age = calculateAge(driver->account_creation);
    driver->avgScore = -1;
    driver->recentRide = NULL;
    driver->scorePerCity = NULL;
    free(aux);
    addAL(list, driver);
    return 1;
}

Driver *findDriverByID(char *id)
{
    return (Driver *)getByIndex(list, atoi(id) - 1);
}

bool isDriverActive(char *id)
{
    return findDriverByID(id)->account_status;
}

char *getDriverBasicInfo(char *id)
{
    Driver *driver = findDriverByID(id);
    char aux[10];
    char *r = (char *)malloc(256);
    strcpy(r, driver->name);
    strcat(r, ";");
    strncat(r, &driver->gender, 1);
    strcat(r, ";");
    sprintf(aux, "%d", driver->age);
    strcat(r, aux);
    strcat(r, ";");
    return r;
}

double getDriverAvgScore(char *id)
{
    Driver *driver = findDriverByID(id);
    if (driver->avgScore == -1)
    {
        driver->avgScore = calculateDriverAvgScore(id);
    }
    return driver->avgScore;
}

double *getDriverAvgScoreAndPay(char *id)
{
    Driver *driver = findDriverByID(id);
    if (driver->avgScore == -1)
        return calculateDriverAvgScoreAndPay(id);

    double *values = (double *)malloc(sizeof(double) * 2);
    values[0] = driver->avgScore;
    values[1] = calculateTotalPayDriver(id);
    return values;
}

char *getCarClass(char *id)
{
    return findDriverByID(id)->car_class;
}

void _freeDriver(void *d)
{
    Driver *driver = (Driver *)d;
    free(driver->birth_day);
    free(driver->account_creation);
    free(driver->id);
    free(driver->car_class);
    free(driver->city);
    free(driver->name);
    if(driver->scorePerCity)
        free(driver->scorePerCity);
    free(driver);
}

void freeDriver()
{
    freeArrayList(list, _freeDriver);
}

int getDriverAccAge(char *id)
{
    return findDriverByID(id)->account_age;
}

char *getDriverName(char *id)
{
    return findDriverByID(id)->name;
}

int compareDriversByScore(const void *A, const void *B)
{
    Driver *a = *(Driver **)A;
    Driver *b = *(Driver **)B;
    if (a->avgScore == -1)
        a->avgScore = calculateDriverAvgScore(a->id);
    if (b->avgScore == -1)
        b->avgScore = calculateDriverAvgScore(b->id);

    if (a->avgScore < b->avgScore)
        return 1;
    if (a->avgScore == b->avgScore)
        return mostRecentRide(a->id, b->id);
    return -1;
}

char **topNdrivers(int n)
{
    ArrayList *temp = copyAL(list, sizeof(Driver *));
    int i, k = 0;
    for (i = 0; i < getALSize(temp); i++)
    {
        if (!getByIndex(temp, i))
        {
            updateArrayList(temp, sizeof(Driver *), i);
            break;
        }
    }
    Driver *driver;
    quickSortArrayList(temp, sizeof(Driver *), compareDriversByScore);
    char **r = malloc(sizeof(char *) * n);
    char aux[10];
    for (i = 0; i < n; i++)
    {
        driver = (Driver *)getByIndex(temp, k);
        if (driver->account_status == false)
        {
            i--;
        }
        else
        {
            r[i] = (char *)malloc(strlen(driver->id) + strlen(driver->name) + 15);
            sprintf(aux, "%.3lf", driver->avgScore);
            strcpy(r[i], driver->id);
            strcat(r[i], ";");
            strcat(r[i], driver->name);
            strcat(r[i], ";");
            strcat(r[i], aux);
        }
        k++;
    }
    freeArrayListSimple(temp);
    return r;
}

bool doesDriverExist(char *id)
{
    long long i = atoi(id);
    return i > 0 && i < getALSize(list) && getByIndex(list, i - 1);
}

Date *getDriverRecentDate(char *id)
{
    return findDriverByID(id)->recentRide;
}

void setDriverRecentDate(char *id, Date *date)
{
    Driver *driver = findDriverByID(id);
    driver->recentRide = date;
}

char getDriverGender(char *id)
{
    return findDriverByID(id)->gender;
}

char *getDriverIDAndName(char *id)
{
    Driver *driver = findDriverByID(id);
    char *s = (char *) malloc(256);
    strcpy(s, driver->id);
    strcat(s, ";");
    strcat(s, driver->name);
    return s;
}

Date *getDriverAccCreation(char *id)
{
    return findDriverByID(id)->account_creation;
}

int getNDrivers()
{
    return getALSize(list);
}

void addDriverRideInCity(char *id, int city)
{
    Driver *driver = findDriverByID(id);
    if(!driver->scorePerCity)
        driver->scorePerCity = (double *) calloc(getNCities(), sizeof(double));
    driver->scorePerCity[city]++;
}

double getDriverNRidesInCity(char *id, int city)
{
    Driver *driver = findDriverByID(id);
    if(!driver->scorePerCity)
        return 0;
    return driver->scorePerCity[city];
}