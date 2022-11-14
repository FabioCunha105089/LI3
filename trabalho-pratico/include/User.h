#ifndef USER_H_
#define USER_H_

#include <stdbool.h>
#include "Date.h"
#include <stdio.h>
#include "ArrayList.h"

typedef struct user User;
User *loadUser(char *);
int getElementSizeUser();
User *findUserArrayID(User **, int, char *);
gboolean isUserActive(User *);
char *getUserName(User *);
char *getUserUsername(User *);
char getUserGender(User *);
Date getUserBirth(User *);
#endif