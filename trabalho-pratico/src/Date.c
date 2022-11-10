#include "Date.h"
#include <string.h>

typedef struct date{
    int day;
    int month;
    int year;
}Date;

Date sToDate(char* string){
    Date date;
    date.day = atoi(strsep(&string, "/"));
    date.month = atoi(strsep(&string, "/"));
    date.year = atoi(strsep(&string, "\0"));
    return date;
}