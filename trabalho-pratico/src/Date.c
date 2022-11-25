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
    if(date->month > BASEMONTH || (date->month == BASEMONTH && date->day > BASEDAY)){
        age--;
    }
    return age;
}


int isDateBigger (Date *dateA, Date *dateB) {

    int date1 = dateA->day + dateA->month * 100 + dateA->year * 2000;
    int date2 = dateB->day + dateB->month * 100 + dateB->year * 2000;

    if(date1 > date2)
        return 1;
    if(date1 == date2)
        return 0;
    return -1;

}