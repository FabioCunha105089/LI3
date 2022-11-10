#include "User.h"
#include "Date.h"
#include "boolean.h"
#include <stdio.h>
#include <string.h>

typedef struct User{
        char* username;
        char* name;
        char gender;
        Date birthdate;
        Date account_creation;
        char* pay_method;
        boolean account_status;
    }User;

User* loadUsers(FILE* file, int size){
    char* line = (char*)malloc(256);
    char* sp;
    User* users = (User*)malloc(sizeof(User) * size);
    fgets(line, 256, file);

    for(int i = 0; i < size; i++){
        fgets(line, 256, file);
        sp = strdup(line);
        users[i].username = strsep(&sp, ";");
        users[i].name = strsep(&sp, ";");
        users[i].gender = strsep(&sp, ";")[0];        
        users[i].birthdate = sToDate(strsep(&sp, ";"));
        users[i].account_creation = sToDate(strsep(&sp, ";"));
        users[i].pay_method = strsep(&sp, ";");
        users[i].account_status = strcmp(strsep(&sp, "\n"), "active") == 0 ? TRUE : FALSE;
    }
    fclose(file);
    return users;
}