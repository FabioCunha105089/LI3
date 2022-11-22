#include "Driver.h"
#include <string.h>
#include <stdlib.h>
#include "ArrayList.h"
#include "Ride.h"

typedef struct driver
{
    char *id;
    char *name;
    Date *birth_day;
    char gender;
    char *car_class;
    char *license_plate;
    char *city;
    Date *account_creation;
    gboolean account_status;
    int age;
    int account_age;
    double avgScore;
} Driver;

static ArrayList *list = NULL;

void initListDriver(int size)
{
    if (!list)
        list = createAL(size - 1, sizeof(Driver *));
}

void loadDriver(char *sp)
{
    Driver *driver = (Driver *)malloc(sizeof(Driver));
    driver->id = strsep(&sp, ";");
    driver->name = strsep(&sp, ";");
    driver->birth_day = sToDate(strsep(&sp, ";"));
    driver->gender = strsep(&sp, ";")[0];
    driver->car_class = strsep(&sp, ";");
    driver->license_plate = strsep(&sp, ";");
    driver->city = strsep(&sp, ";");
    driver->account_creation = sToDate(strsep(&sp, ";"));
    driver->account_status = strcmp(strsep(&sp, "\n"), "active") == 0 ? TRUE : FALSE;
    driver->age = calculateAge(driver->birth_day);
    driver->account_age = calculateAge(driver->account_creation);
    driver->avgScore = -1;
    addAL(list, driver);
}

Driver *findDriverByID(char *id)
{
    return (Driver *)getByIndex(list, atoi(id) - 1);
}

gboolean isDriverActive(char *id)
{
    return findDriverByID(id)->account_status;
}

char *getDriverBasicInfo(char *id)
{
    Driver *driver = findDriverByID(id);
    char aux[100];
    char *r = driver->name;
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

    double *values = (double *) malloc (sizeof(double) * 2);
    values[0] = driver->avgScore;
    values[1] = calculateTotalPayDriver(id);
    return values;
}

char *getCarClass(char *id)
{
    return findDriverByID(id)->car_class;
}

void freeDriver(){ 
    freeArrayList(list);
}

int getDriverAccAge(char *id)
{
    return findDriverByID(id)->account_age;
}

char *getDriverName(char * id)
{
    return findDriverByID(id)->name;
}