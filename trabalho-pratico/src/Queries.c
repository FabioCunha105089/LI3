#include "Queries.h"
#include "Driver.h"
#include "Ride.h"
#include "User.h"
#include <stdlib.h>
#include <string.h>
#include "ArrayList.h"

double getPrice(char* car_class, float distance){
    if (strcmp(car_class, "green") == 0){
        return 4 + distance * 0.79;
    }
    if(strcmp(car_class, "premium") == 0){
        return 5.2 + distance * 0.94;
    }
    return 3.25 + distance * 0.62;
}

double query4(ArrayList* rides, int size, ArrayList* drivers,  char* city){
    double tPrice = 0;
    int nRides = 0;
    int id;
    for(int i = 0; i < size; i++){
        if(strcmp(rides[i].city, city) == 0){
            id = atoi(rides[i].driver) - 1;
            tPrice += getPrice(drivers[id].car_class, rides[i].distance);
            nRides++;
        }
    }
    if(nRides == 0){
        return 0;
    }
    return tPrice / nRides;
}
