#ifndef _USER_H_
#define _USER_H_

    #include <stdbool.h>
    #include "Date.h"
    #include <stdio.h>
    #include "ArrayList.h"

    typedef struct user* User;
    User* loadUser(char*);
    int getElementSizeUser();
#endif