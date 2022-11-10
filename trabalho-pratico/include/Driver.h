#ifndef _DRIVER_H_
#define _DRIVER_H_

    #include "Date.h"
    typedef enum { FALSE, TRUE } bool;

    typedef struct Driver{
        char* id;
        char* name;
        Date birth_day;
        char gender;
        char* car_class;
        char* license_plate;
        char* city;
        Date account_creation;
        bool account_status;
    }Driver;

#endif