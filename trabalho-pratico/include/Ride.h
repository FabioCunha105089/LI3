#ifndef RIDE_H_
#define RIDE_H_

#include "Date.h"
#include <stdio.h>
#include <glib.h>
#include "LinkedList.h"

typedef struct ride Ride;
void loadRide(char *);
int getElementSizeRide();
LinkedList *getRideHashTables(void **, int);
double avgPayInCity(char *);
gboolean doesDriverHaveRides(char *);
int getNumberOfRidesDriver(char *);
double calculateTotalPay(char *);
double calculateDriverAvgScore(char *);
double* calculateDriverAvgScoreAndPay(char *);
void initRide(int);
void initHashTables();

#endif
