#include "Date.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

typedef struct date
{
    int day;
    int month;
    int year;
} Date;

Date *sToDate(char* string, int size){
    if(size != 10)
        return NULL;
    if(isdigit(string[0]) == 0 || isdigit(string[1]) == 0 || string[2] != '/' 
        || isdigit(string[3]) == 0 || isdigit(string[4]) == 0 || string[5] != '/' 
        || isdigit(string[6]) == 0 || isdigit(string[7]) == 0 || isdigit(string[8]) == 0 
        || isdigit(string[9]) == 0)
        return NULL;
    Date *date = (Date *) malloc(sizeof(Date));
    date->day = atoi(strsep(&string, "/"));
    date->month = atoi(strsep(&string, "/"));
    if(date->day > 31 || date->day < 1 || date->month < 1 || date->month > 12)
    {
        free(date);
        return NULL;
    }
    date->year = atoi(strsep(&string, "\0"));

    return date;
}

bool isDateValid(Date *data){

    int dataA = data->day + data->month * 100 + data->year * 2000;
    int dataB = 9 + 10 * 100 + 2022 * 2000;

    if (dataA <= dataB) return true;

    return false;
}

Date *sToDateSimple(char* string){
    Date *date = (Date *) malloc(sizeof(Date));
    date->day = atoi(strsep(&string, "/"));
    date->month = atoi(strsep(&string, "/"));
    date->year = atoi(strsep(&string, "\0"));

    return date;
}

char *dateToS(Date *data){
    char * r = (char *)malloc(11);
    char aux[10];
    sprintf(aux, "%i", data->day);
    if(data->day < 10)
    {
        strcpy(r, "0");
        strcat(r, aux);
    }
    else strcpy(r, aux);
    strcat(r, "/");
    sprintf(aux, "%i", data->month);
    if(data->month < 10)
    {
        strcat(r, "0");
        strcat(r, aux);
    }
    else strcat(r, aux);
    strcat(r, "/");
    sprintf(aux, "%i", data->year);
    strcat(r, aux);
    strcat(r, "\0");

    return r;
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

void printDate(Date *data)
{
    if(data)
        printf("%d/%d/%d\n", data->day, data->month, data->year);
}