#ifndef USER_H_
#define USER_H_

#include <stdbool.h>
#include "Date.h"
#include <stdio.h>
#include "ArrayList.h"

typedef struct user User;
void loadUser(char *);
int getElementSizeUser();
User *findUserByUsername(char *);
gboolean isUserActive(User *);
void initListUser(int);
char *getUserBasicInfo(char *);
double getUserAvgScore(char *);
double *getUserAvgScoreAndPay(char *);
void freeUser();
#endif