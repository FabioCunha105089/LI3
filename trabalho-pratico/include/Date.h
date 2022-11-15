#ifndef DATE_H_
#define DATE_H_

typedef struct date
{
    int day;
    int month;
    int year;
} Date;

#define BASEDAY 19
#define BASEMONTH 10
#define BASEYEAR 2022

Date *sToDate(char *);
int calculateAge(Date *);

#endif