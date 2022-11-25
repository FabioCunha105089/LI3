#ifndef RIDE_H_
#define RIDE_H_

#include "Date.h"
#include <stdio.h>
#include <glib.h>
#include "LinkedList.h"

typedef struct ride Ride;
void loadRide(char *);
double avgPayInCity(char *);
gboolean doesDriverHaveRides(char *);
int getNumberOfRidesDriver(char *);
double calculateTotalPayDriver(char *);
double calculateDriverAvgScore(char *);
double* calculateDriverAvgScoreAndPay(char *);
gboolean doesUserHaveRides(char *);
int getNumberOfRidesUser(char *);
double calculateTotalPayUser(char *);
double calculateUserAvgScore(char *);
double* calculateUserAvgScoreAndPay(char *);
void initListRide(int);
void initHashTables();
void freeRide();
double avgDistanceInCityByDate(char *, char*, char*);
ArrayList *getRidesInCityByDriverScore(char *city);
char *getDriverIDFromRide(Ride *);
char *getDriverNameFromRide(Ride *);
double getDriverAvgScoreInCityFromRide(Ride *);
int mostRecentRide(char *, char *);


#endif
