#include "Queries.h"
#include "Driver.h"
#include "Ride.h"
#include "User.h"
#include <stdlib.h>
#include <string.h>
#include "ArrayList.h"
#include <glib.h>
#include "LinkedList.h"

double getPrice(char* car_class, float distance){
    if (strcmp(car_class, "green") == 0){
        return 4 + distance * 0.79;
    }
    if(strcmp(car_class, "premium") == 0){
        return 5.2 + distance * 0.94;
    }
    return 3.25 + distance * 0.62;
}

double query4(GHashTable* hash, ArrayList* drivers, char* city){
    if(g_hash_table_contains(hash, city) == FALSE) return 0;

    double tPrice = 0;
    int id, nRides;
    Ride* ride;
    LinkedList* list = (LinkedList*) g_hash_table_find(hash, true, city);
    nRides = getLLSize(list);
    for(int i = 0; i < nRides; i++){
        ride = findByIndex(list, i);
        id = getDriver(ride);
        tPrice += getPrice(getCarClass(find(drivers, id, *findDriverArrayID)), getDistance(ride));
    }
    return tPrice / nRides;
}