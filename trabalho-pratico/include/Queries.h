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
void executeQuery(char, char **);
#endif
