#ifndef _DRIVER_H_
#define _DRIVER_H_

    #include "Date.h"
    #include "boolean.h"
    #include <stdio.h>

    typedef struct driver* Driver;
    Driver* loadDriver(char*);
    int getElementSizeDriver();
    Driver findDriverArrayID(Driver* array, int arrSize, char* id);
    char* getCarClass(Driver* driver);

#endif