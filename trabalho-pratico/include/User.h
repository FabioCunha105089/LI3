#ifndef _USER_H_
#define _USER_H_

    #include <stdbool.h>
    #include "Date.h"

    typedef struct User{
        char* username;
        char* name;
        char gender;
        Date birthdate;
        Date account_creation;
        char* pay_method;
        bool account_status;
    }User;

#endif