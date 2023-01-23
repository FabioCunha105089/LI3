#ifndef DRIVER_H_
#define DRIVER_H_

#include "Date.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct driver Driver;
int loadDriver(char *);
Driver* findDriverByID(char *);
char *getCarClass(char *);
bool isDriverActive(char *);
void initListDriver(int);
char *getDriverBasicInfo(char *);
double getDriverAvgScore(char *);
double *getDriverAvgScoreAndPay(char *);
void freeDriver();
int getDriverAccAge(char *);
char *getDriverName(char *);
char **topNdrivers(int);
bool doesDriverExist(char *);
Date *getDriverRecentDate(char *);
void setDriverRecentDate(char *, Date *);
char getDriverGender(char *);
char *getDriverIDAndName(char *);
Date *getDriverAccCreation(char *);
int getNDrivers();

#endif