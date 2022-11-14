#include "User.h"
#include "Date.h"
#include "boolean.h"
#include <stdio.h>
#include <string.h>

typedef struct User
{
    char *username;
    char *name;
    char gender;
    Date birthdate;
    Date account_creation;
    char *pay_method;
    boolean account_status;
} User;

int getElementSizeUser()
{
    return sizeof(User *);
}

User *loadUser(char * sp)
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