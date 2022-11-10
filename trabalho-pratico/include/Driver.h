#ifndef DRIVER_H_
#define DRIVER_H_

#include "Date.h"
#include <stdio.h>
#include <glib.h>

typedef struct driver Driver;
Driver *loadDriver(char *);
int getElementSizeDriver();
Driver* findDriverArrayID(Driver **, int, char *);
char *getCarClass(Driver *);
gboolean isDriverActive(Driver *);
char *getDriverID(Driver *);
char *getDriverName(Driver *);
char getDriverGender(Driver *);
Date getDriverBirth(Driver *);
char *findDriverCarClass(Driver **, int, char *);

#endif