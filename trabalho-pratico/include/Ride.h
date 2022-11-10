#ifndef _RIDE_H_
#define _RIDE_H_

    #include "Date.h"
    #include "Driver.h"
    #include <stdio.h>

    typedef struct ride* Ride;

    Ride* loadRides(FILE*, int);
#endif