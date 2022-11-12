#ifndef QUERIES_H_
#define QUERIES_H_
#include "Driver.h"
#include <glib.h>
#include "ArrayList.h"

typedef struct query Query;

void executeQueries();
void loadQuery(char *);
void initListQuery(int);

#endif