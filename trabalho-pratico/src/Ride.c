#include "Ride.h"
#include "stdio.h"
#include "Date.h"
#include "boolean.h"
#include <string.h>

typedef struct ride{
        char* id;
        Date date;
        char* driver;
        char* user;
        char* city;
        float distance;
        int score_user;
        int score_driver;
        float tip;
        char* comment;
    }Ride;

Ride* loadRides(FILE* file, int size){
    char* line = (char*)malloc(256);
    char* sp;
    ArrayList* list = create(size, sizeof(Ride));
    fgets(line, 256, file);
    for(int i = 0; i < size; i++){
        fgets(line, 256, file);
        sp = strdup(line);
        rides[i].id = strsep(&sp, ";");
        rides[i].date = sToDate(strsep(&sp, ";"));
        rides[i].driver = strsep(&sp, ";");
        rides[i].user = strsep(&sp, ";");        
        rides[i].city = strsep(&sp, ";");
        rides[i].distance = atof(strsep(&sp, ";"));
        rides[i].score_user = atoi(strsep(&sp, ";"));
        rides[i].score_driver = atoi(strsep(&sp, ";"));
        rides[i].tip = atof(strsep(&sp, ";"));
        rides[i].comment = strsep(&sp, "\n");
    }
    fclose(file);
    return rides;
}