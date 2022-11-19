#ifndef QUERIES_H_
#define QUERIES_H_
#include <glib.h>
#include "LinkedList.h"

typedef struct query Query;

void executeQueries();
void loadQuery(char *);
void initListQuery(int);
void freeQuery();
int getNArgs(char);
double query4(char *);
double query6(char *, char *, char *);
char *query1_drivers(char *);
char *query1_users(char *);
char *query1(char *);
void swap(char *, char *);
char *_q8getDriverID(char *);
char *_q8getUsername(char *);
char *_q8getRideID(char *);
int _q8CheckPivot(char*, int, char *, int);
int partition(char **, int, int);
void quickSort(char **, int, int);
char *_q8removeRideID(char *);
LinkedList *query8(char, int);
#endif