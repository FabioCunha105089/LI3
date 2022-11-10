#include "Driver.h"
#include <stdio.h>
#include "Date.h"
#include <string.h>
#include "boolean.h"
#include <stdlib.h>
#include "ArrayList.h"

typedef struct Driver{
        char* id;
        char* name;
        Date birth_day;
        char gender;
        char* car_class;
        char* license_plate;
        char* city;
        Date account_creation;
        boolean account_status;
    }Driver;

ArrayList* loadDrivers(FILE* file, int size){
    char* line = (char*)malloc(sizeof(char) * 256);
    char* sp;
    ArrayList* list = create(size, sizeof(Driver));
    fgets(line, 256, file);
    for(int i = 0; i < size; i++){
        fgets(line, 256, file);
        sp = strdup(line);
        Driver* driver = (Driver*)malloc(sizeof(Driver));
        driver->id = strsep(&sp, ";");
        driver->name = strsep(&sp, ";");
        driver->birth_day = sToDate(strsep(&sp, ";"));
        driver->gender = strsep(&sp, ";")[0];
        driver->car_class = strsep(&sp, ";");
        driver->license_plate = strsep(&sp, ";");
        driver->city = strsep(&sp, ";");
        driver->account_creation = sToDate(strsep(&sp, ";"));
        driver->account_status = strcmp(strsep(&sp, "\n"), "active") == 0 ? TRUE : FALSE;
        add(list, driver);
    }
    return list;
}

Driver findDriverArrayID(Driver* array, int arrSize, char* id){
    return array[atoi(id) - 1];
}