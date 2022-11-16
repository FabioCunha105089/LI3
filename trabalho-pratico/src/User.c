#include "User.h"
#include "Date.h"
#include <stdio.h>
#include <string.h>

typedef struct user {

    char *username;
    char *name;
    char gender;
    Date *birthdate;
    Date *account_creation;
    char *pay_method;
    gboolean account_status;
    int age;
    double avgScore;
} User;

static ArrayList *list = NULL;

int getElementSizeUser() {

    return sizeof(User *);
}

void initListUser(int size) {
    
    if (!list)
        list = createAL(size, sizeof(User *));
}

void loadUser(char *sp) {

    User *user = (User *)malloc(sizeof(User));
    user->username = strsep(&sp, ";");
    user->name = strsep(&sp, ";");
    user->gender = strsep(&sp, ";")[0];
    user->birthdate = sToDate(strsep(&sp, ";"));
    user->account_creation = sToDate(strsep(&sp, ";"));
    user->pay_method = strsep(&sp, ";");
    user->account_status = strcmp(strsep(&sp, "\n"), "active") == 0 ? TRUE : FALSE;
    user->age = calculateAge(user->birthdate);
    user->avgScore = -1;
    addAL(list, user);
}

User *findUserByUsername(char *username) {

    User * user;
    int arrSize = getALSize(list);
    for (int i = 0; i < arrSize; i++) {
        
        user = (User *)getByIndex(list,i);
        
        if (strcmp(user->username, username) == 0) {
            return user;
        }
    }
}


gboolean isUserActive(User *user) {

    return user->account_status;
}

char *getUserBasicInfo(char *id) {

    User *user = findUserByUsername(id);
    char aux[100];
    char *r = user->name;
    strcat(r, ";");
    strncat(r, &user->gender, 1);
    strcat(r, ";");
    sprintf(aux, "%d", user->age);
    strcat(r, aux);
    strcat(r, ";");
    return r;
}


double getUserAvgScore(char *id) {

    User *user = findUserByUsername(id);
    
    if (user->avgScore == -1) {
        user->avgScore = calculateUserAvgScore(id);
    }

    return user->avgScore;

}

double *getUserAvgScoreAndPay(char *id) {

    User *user = findUserByUsername(id);
    
    if (user->avgScore == -1) {
        return calculateUserAvgScoreAndPay(id);
    }

    double *values = (double *) malloc (sizeof(double)*2);
    values[0] = user->avgScore;
    values[1] = calculateTotalPayUser(id);
    return values;

}

void freeUser()
{
    freeArrayList(list);
}