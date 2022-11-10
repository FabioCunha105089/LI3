#ifndef _DRIVER_H_
#define _DRIVER_H_

    #include "Date.h"
    #include "boolean.h"
    #include <stdio.h>

    typedef struct driver* Driver;
    ArrayList* loadDrivers(FILE*, int);

#endif