#ifndef QUERIES_H_
#define QUERIES_H_
#include "Driver.h"
#include <glib.h>
#include "ArrayList.h"
void query2(Driver *, int);
double query4(GHashTable *, ArrayList *, char *);
char *query1(char *, GHashTable *, GHashTable *, ArrayList *, ArrayList *);
#endif