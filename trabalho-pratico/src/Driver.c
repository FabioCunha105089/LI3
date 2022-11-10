#include "Driver.h"
#include <string.h>
#include <stdlib.h>
#include "ArrayList.h"

typedef struct driver
{
    char *id;
    char *name;
    Date birth_day;
    char gender;
    char *car_class;
    char *license_plate;
    char *city;
    Date account_creation;
    gboolean account_status;
} Driver;

int getElementSizeDriver()
{
    return sizeof(Driver *);
}

Driver *loadDriver(char *sp)
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
    return driver;
}

Driver* findDriverArrayID(Driver **array, int arrSize, char *id)
{
    return array[atoi(id) - 1];
}

char *getCarClass(Driver *driver)
{
    return driver->car_class;
}

gboolean isDriverActive(Driver* driver){
    return driver->account_status;
}

char* getDriverID(Driver* driver){
    return driver->id;
}

char* getDriverName(Driver* driver){
    return driver->name;
}

char getDriverGender(Driver* driver){
    return driver->gender;
}

Date getDriverBirth(Driver* driver){
    return driver->birth_day;
}

char *findDriverCarClass(Driver **array, int arrSize, char *id){
    return array[atoi(id) - 1]->car_class;
}