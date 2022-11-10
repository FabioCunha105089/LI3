#ifndef _RIDE_H_
#define _RIDE_H_

    #include "Date.h"
    #include "Driver.h"

    typedef struct Ride{
        char* id;
        Date date;
        Driver* driver;
        char* user;
        char* city;
        float distance;
        int score_user;
        int score_driver;
        float tip;
        char* comment;
    }Ride;

#endif