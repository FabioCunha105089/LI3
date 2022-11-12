#ifndef DRIVER_H_
#define DRIVER_H_

#include "Date.h"
#include <stdio.h>
#include <glib.h>

typedef struct driver Driver;
void loadDriver(char *);
int getElementSizeDriver();
Driver* findDriverByID(char *);
char *getCarClass(Driver *);
gboolean isDriverActive(char *);
char *getDriverID(Driver *);
char *getDriverName(Driver *);
char getDriverGender(Driver *);
Date getDriverBirth(Driver *);
char *findDriverCarClass(Driver **, int, char *);
void initListDriver(int);
char *getDriverBasicInfo(char *);
double getDriverAvgScore(char *);
double *getDriverAvgScoreAndPay(char *);
void initListDriver(int);

#endif