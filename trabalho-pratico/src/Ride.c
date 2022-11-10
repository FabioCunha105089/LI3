#include "Ride.h"
#include "stdio.h"
#include "Date.h"
#include "boolean.h"
#include <string.h>
#include "LinkedList.h"

typedef struct ride
{
    char *id;
    Date date;
    char *driver;
    char *user;
    char *city;
    float distance;
    int score_user;
    int score_driver;
    float tip;
    char *comment;
} Ride;

int getElementSizeRide(){
    return sizeof(Ride*);
}

Ride *loadRide(char *sp){
    Ride *ride = (Ride *)malloc(sizeof(Ride));
    ride->id = strsep(&sp, ";");
    ride->date = sToDate(strsep(&sp, ";"));
    ride->driver = strsep(&sp, ";");
    ride->user = strsep(&sp, ";");
    ride->city = strsep(&sp, ";");
    ride->distance = atof(strsep(&sp, ";"));
    ride->score_user = atoi(strsep(&sp, ";"));
    ride->score_driver = atoi(strsep(&sp, ";"));
    ride->tip = atof(strsep(&sp, ";"));
    ride->comment = strsep(&sp, "\n");
    return ride;
}

GHashTable* getCityHash(Ride** rides, int size){
    GHashTable* hash = g_hash_table_new(g_str_hash, g_str_equal);
    for(int i = 0; i < size; i++){
        if(g_hash_table_contains(hash, rides[i]->city) == FALSE){
            LinkedList* list = create();
            add(list, rides[i]);
            g_hash_table_insert(hash, rides[i]->city, list);
        } else {
            add((LinkedList*) g_hash_table_find(hash,TRUE, rides[i]->city), rides[i]);
        }
    }
    return hash;
}

char* getDriver(Ride* ride){
    return ride->driver;
}

float getDistance(Ride* ride){
    return ride->distance;
}