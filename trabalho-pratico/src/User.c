#include "User.h"
#include "Date.h"
#include "Ride.h"
#include <string.h>
#include "ArrayList.h"
#include <glib.h>

typedef struct user
{

    char *username;
    char *name;
    char gender;
    Date *birthdate;
    Date *account_creation;
    char *pay_method;
    gboolean account_status;
    int age;
    int account_age;
    double avgScore;
} User;

static ArrayList *list = NULL;
static GHashTable *positions = NULL;

void initListUser(int size)
{
    if (!list)
        list = createAL(size - 1, sizeof(User *));
}

void loadUser(char *sp)
{

    if (!positions)
        positions = g_hash_table_new(g_str_hash, g_str_equal);

    User *user = (User *)malloc(sizeof(User));
    user->username = strdup(strsep(&sp, ";"));
    user->name = strdup(strsep(&sp, ";"));
    user->gender = strsep(&sp, ";")[0];
    user->birthdate = sToDate(strsep(&sp, ";"));
    user->account_creation = sToDate(strsep(&sp, ";"));
    user->pay_method = strdup(strsep(&sp, ";"));
    user->account_status = strcmp(strsep(&sp, "\n"), "active") == 0 ? TRUE : FALSE;
    user->age = calculateAge(user->birthdate);
    user->account_age = calculateAge(user->account_creation);
    user->avgScore = -1;
    g_hash_table_insert(positions, user->username, user);
    addAL(list, user);
}

User *findUserByUsername(char *username)
{
    if(g_hash_table_contains(positions, username) == FALSE)
        return NULL;
    return (User *)g_hash_table_lookup(positions, username);
}

gboolean isUserActive(char *username)
{
    if(g_hash_table_contains(positions, username) == FALSE)
        return FALSE;
    return findUserByUsername(username)->account_status;
}

char *getUserBasicInfo(char *id)
{

    User *user = findUserByUsername(id);
    char aux[100];
    char *r = (char *) malloc (256);
    strcat(r,user->name);
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