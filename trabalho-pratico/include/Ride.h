#ifndef RIDE_H_
#define RIDE_H_

#include "Date.h"
#include <stdio.h>
#include <glib.h>
#include "LinkedList.h"

typedef struct ride Ride;
void *loadRide(char *);
int getElementSizeRide();
LinkedList *getRideHashTables(void **, int);
char *getDriver(Ride *);
double getDistance(Ride *);
int getScoreDriver(Ride *);
double getTip(Ride *);

#endif