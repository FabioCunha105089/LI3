#include "Date.h"
#include <stdlib.h>
#include <string.h>

Date *sToDate(char* string){
    Date *date = (Date *) malloc(sizeof(Date));
    date->day = atoi(strsep(&string, "/"));
    date->month = atoi(strsep(&string, "/"));
    date->year = atoi(strsep(&string, "\0"));

    return date;
}

int calculateAge(Date *date){
    int age = 0;
    age = BASEYEAR - date->year;
    if(date->month < BASEMONTH || (date->month == BASEMONTH && date->day < BASEDAY)){
        age--;
    }
    return age;
}