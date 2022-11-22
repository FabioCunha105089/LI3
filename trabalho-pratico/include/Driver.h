#ifndef DRIVER_H_
#define DRIVER_H_

#include "Date.h"
#include <stdio.h>
#include <glib.h>

typedef struct driver Driver;
void loadDriver(char *);
Driver* findDriverByID(char *);
char *getCarClass(char *);
gboolean isDriverActive(char *);
void initListDriver(int);
char *getDriverBasicInfo(char *);
double getDriverAvgScore(char *);
double *getDriverAvgScoreAndPay(char *);
void freeDriver();
int getDriverAccAge(char *);
char *getDriverName(char *);

#endif