#ifndef DRIVER_H_
#define DRIVER_H_

#include "Date.h"
#include <stdio.h>
#include <glib.h>

typedef struct driver Driver;
void loadDriver(char *);
int getElementSizeDriver();
Driver* findDriverByID(char *);
char *getCarClass(char *);
gboolean isDriverActive(char *);
char *findDriverCarClass(Driver **, int, char *);
void initListDriver(int);
char *getDriverBasicInfo(char *);
double getDriverAvgScore(char *);
double *getDriverAvgScoreAndPay(char *);
void initListDriver(int);
void freeDriver();
char *driverGenderAndAccountAge(char *, char, int);
int getDriverAccAge(char *);
char *getDriverName(char *);
char getDriverGender(char *);

#endif