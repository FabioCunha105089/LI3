#include "Ride.h"
#include "stdio.h"
#include "Date.h"
#include <string.h>
#include "LinkedList.h"

typedef struct ride
{
    char *id;
    Date date;
    char *driver;
    char *user;
    char *city;
    double distance;
    int score_user;
    int score_driver;
    double tip;
    char *comment;
} Ride;

int getElementSizeRide()
{
    return sizeof(Ride *);
}

void *loadRide(char *sp)
{
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

// GHashTable* getCityHash(Ride** rides, int size){
//     GHashTable* hash = g_hash_table_new(g_str_hash, g_str_equal);
//     for(int i = 0; i < size; i++){
//         if(g_hash_table_contains(hash, rides[i]->city) == FALSE){
//             LinkedList* list = create();
//             add(list, rides[i]);
//             g_hash_table_insert(hash, rides[i]->city, list);
//         } else {
//             add((LinkedList*) g_hash_table_find(hash,TRUE, rides[i]->city), rides[i]);
//         }
//     }
//     return hash;
// }

LinkedList *getRideHashTables(void **data, int size)
{
    Ride **rides = (Ride **)data;
    GHashTable *hashCity = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTable *hashDriver = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTable *hashUsers = g_hash_table_new(g_str_hash, g_str_equal);
    LinkedList *listHash = createLL();

    for (int i = 0; i < size; i++)
    {
        // Cidades
        if (g_hash_table_contains(hashCity, rides[i]->city) == FALSE)
        {
            LinkedList *list = createLL();
            addLL(list, rides[i]);
            g_hash_table_insert(hashCity, rides[i]->city, list);
        }
        else
        {
            addLL((LinkedList *)g_hash_table_lookup(hashCity, rides[i]->city), rides[i]);
        }

        // Drivers
        if (g_hash_table_contains(hashDriver, rides[i]->driver) == FALSE)
        {
            LinkedList *list = createLL();
            addLL(list, rides[i]);
            g_hash_table_insert(hashDriver, rides[i]->driver, list);
        }
        else
        {
            addLL((LinkedList *)g_hash_table_lookup(hashDriver, rides[i]->driver), rides[i]);
        }

        // Users
        if (g_hash_table_contains(hashUsers, rides[i]->user) == FALSE)
        {
            LinkedList *list = createLL();
            addLL(list, rides[i]);
            g_hash_table_insert(hashUsers, rides[i]->user, list);
        }
        else
        {
            addLL((LinkedList *)g_hash_table_lookup(hashUsers, rides[i]->user), rides[i]);
        }
    }

    addLL(listHash, hashCity);
    addLL(listHash, hashDriver);
    addLL(listHash, hashUsers);
    return listHash;
}

char *getDriver(Ride *ride)
{
    return ride->driver;
}

double getDistance(Ride *ride)
{
    return ride->distance;
}

int getScoreDriver(Ride *ride)
{
    return ride->score_driver;
}

double getTip(Ride *ride)
{
    return ride->tip;
}

int getScoreUser(Ride *ride)
{
    return ride->score_user;
}