#include "User.h"
#include "Date.h"
#include "Ride.h"
#include <string.h>
#include "ArrayList.h"
#include <glib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct user
{

    char *username;
    char *name;
    char gender;
    Date *birthdate;
    Date *account_creation;
    char *pay_method;
    bool account_status;
    int age;
    int account_age;
    double avgScore;
    int totalDist;
} User;

static ArrayList *list = NULL;
static GHashTable *positions = NULL;

void initListUser(int size)
{
    if (!list)
        list = createAL(size - 1, sizeof(User *));
    if (!positions)
        positions = g_hash_table_new(g_str_hash, g_str_equal);
}

int loadUser(char *sp)
{
    User *user = (User *)malloc(sizeof(User));
    char *aux;
    user->username = strdup(strsep(&sp, ";"));
    if(strlen(user->username) == 0)
    {
        free(user->username);
        free(user);
        return 0;
    }
    user->name = strdup(strsep(&sp, ";"));
    if(strlen(user->name) == 0)
    {
        free(user->username);
        free(user->name);
        free(user);
        return 0;
    }
    user->gender = strsep(&sp, ";")[0];
    if(user->gender == '\0')
    {
        free(user->username);
        free(user->name);
        free(user);
        return 0;
    }
    aux = strdup(strsep(&sp, ";"));
    user->birthdate = sToDate(aux, strlen(aux));
    if(!user->birthdate)
    {
        free(user->username);
        free(user->name);
        free(user);
        free(aux);
        return 0;
    }
    free(aux);
    aux = strdup(strsep(&sp, ";"));
    user->account_creation = sToDate(aux, strlen(aux));
    if(!user->account_creation)
    {
        free(user->username);
        free(user->name);
        free(user->birthdate);
        free(user);
        free(aux);
        return 0;   
    }
    free(aux);
    user->pay_method = strdup(strsep(&sp, ";"));
    if(strlen(user->pay_method) == 0)
    {
        free(user->username);
        free(user->name);
        free(user->birthdate);
        free(user->account_creation);
        free(user->pay_method);
        free(user);
        return 0;   
    }
    aux = strdup(strsep(&sp, "\n"));
    for(int i = 0; i < strlen(aux); i++)
    {
        aux[i] = tolower(aux[i]);
    }
    if(strcmp(aux, "active") == 0)
    {
        user->account_status = true;
    }
    else if (strcmp(aux, "inactive") == 0)
    {
        user->account_status = false;
    }
    else
    {
        free(aux);
        free(user->username);
        free(user->name);
        free(user->birthdate);
        free(user->account_creation);
        free(user->pay_method);
        free(user);
        return 0;  
    }
    user->age = calculateAge(user->birthdate);
    user->account_age = calculateAge(user->account_creation);
    user->avgScore = -1;
    user->totalDist = 0;
    g_hash_table_insert(positions, user->username, user);
    free(aux);
    addAL(list, user);
    return 1;
}

User *findUserByUsername(char *username)
{
    return (User *)g_hash_table_lookup(positions, username);
}

gboolean isUserActive(char *username)
{
    return findUserByUsername(username)->account_status;
}

char *getUserBasicInfo(char *id)
{
    User *user = findUserByUsername(id);
    char aux[10];
    char *r = (char *) malloc (256);
    strcpy(r,user->name);
    strcat(r, ";");
    strncat(r, &user->gender, 1);
    strcat(r, ";");
    sprintf(aux, "%d", user->age);
    strcat(r, aux);
    strcat(r, ";");
    return r;
}

double getUserAvgScore(char *id)
{

    User *user = findUserByUsername(id);

    if (user->avgScore == -1)
    {
        user->avgScore = calculateUserAvgScore(id);
    }

    return user->avgScore;
}

double *getUserAvgScoreAndPay(char *id)
{

    User *user = findUserByUsername(id);

    if (user->avgScore == -1)
    {
        return calculateUserAvgScoreAndPay(id);
    }

    double *values = (double *)malloc(sizeof(double) * 2);
    values[0] = user->avgScore;
    values[1] = calculateTotalPayUser(id);
    return values;
}

void _freeUser(void *u)
{
    User *user = (User *) u;
    free(user->account_creation);
    free(user->birthdate);
    free(user->name);
    free(user->username);
    free(user->pay_method);
    free(user);
}

void freeUser()
{
    freeArrayList(list, _freeUser);
    g_hash_table_destroy(positions);
}

LinkedList *getUserGenderAccAgeName(char *username)
{
    User *user = findUserByUsername(username);
    LinkedList *r = createLL();
    addLL(r, &user->gender);
    addLL(r, &user->account_age);
    addLL(r, user->name);
    return r;
}

int getUserAccAge(char *username)
{
    return findUserByUsername(username)->account_age;
}

int compareUsersByDistance(const void *A, const void *B)
{
    User *a = *(User **) A;
    User *b = *(User **) B;

    if(a->totalDist == 0 && isUserActive(a->username) == true)
        a->totalDist = calculateUserTotalDist(a->username);

    if(b->totalDist == 0 && isUserActive(b->username) == true)
        b->totalDist = calculateUserTotalDist(b->username);
    if (a->totalDist <= b->totalDist)
        return 1;
    if (a->totalDist == b->totalDist)
        return mostRecentRide(a->username, b->username);
    return -1;
}

char **mostDistUsers(int n)
{
    ArrayList* temp = copyAL(list, sizeof(User *));
    quickSortArrayList(temp, sizeof(User *), compareUsersByDistance);
    char **r = malloc(sizeof(char *) * n);
    char aux[10];
    User *user;
    for (int i = 0; i < n; i++)
    {
        user = (User *)getByIndex(temp, i);
        r[i] = (char *)malloc(strlen(user->username) + strlen(user->name) + 15);
        sprintf(aux, "%d", user->totalDist);
        strcpy(r[i], user->username);
        strcat(r[i], ";");
        strcat(r[i], user->name);
        strcat(r[i], ";");
        strcat(r[i], aux);
    }
    freeArrayListSimple(temp);
    return r;
}

gboolean doesUserExist(char *username)
{
    return g_hash_table_contains(positions, username);
}

void updateUser(int newSize)
{
    updateArrayList(list, sizeof(User *), newSize);
}