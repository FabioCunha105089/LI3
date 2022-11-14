#include "../include/User.h"
#include "../include/Date.h"
#include <stdio.h>
#include <string.h>

typedef struct user
{
    char *username;
    char *name;
    char gender;
    Date birthdate;
    Date account_creation;
    char *pay_method;
    gboolean account_status;
} User;

int getElementSizeUser()
{
    return sizeof(User *);
}

User *loadUser(char *sp)
{
    User *user = (User *)malloc(sizeof(User));
    user->username = strsep(&sp, ";");
    user->name = strsep(&sp, ";");
    user->gender = strsep(&sp, ";")[0];
    user->birthdate = sToDate(strsep(&sp, ";"));
    user->account_creation = sToDate(strsep(&sp, ";"));
    user->pay_method = strsep(&sp, ";");
    user->account_status = strcmp(strsep(&sp, "\n"), "active") == 0 ? TRUE : FALSE;
    return user;
}

User *findUserArrayID(User **array, int arrSize, char *username)
{
    for (int i = 0; i < arrSize; i++)
    {
        if (array[i]->username = username)
        {
            return array[i];
        }
    }
}

gboolean isUserActive(User *user)
{
    return user->account_status;
}

char getUserGender(User *user)
{
    return user->gender;
}

Date getUserBirth(User *user)
{
    return user->birthdate;
}

char *getUserUsername(User *user)
{
    return user->username;
}

char *getUserName(User *user){
    return user->name;
}