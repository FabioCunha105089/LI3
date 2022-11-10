#ifndef _RIDE_H_
#define _RIDE_H_

    #include "Date.h"
    #include <stdio.h>
    #include <glib.h>

    typedef struct ride* Ride;
    Ride* loadRide(char*);
    int getElementSizeRide();
    GHashTable* getCityHash(Ride**, int);
    char* getDriver(Ride* ride);
    float getDistance(Ride* ride);
    
#endif