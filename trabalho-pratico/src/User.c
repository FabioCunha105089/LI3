#include "User.h"
#include "Date.h"
#include "Ride.h"
#include <string.h>
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
    bool account_status;
    int age;
    int account_age;
    double avgScore;
    int totalDist;
    Date *recentRide;
} User;

static GHashTable *hash = NULL;

void _freeUser(gpointer u)
{
    User *user = (User *)u;
    free(user->account_creation);
    free(user->birthdate);
    free(user->name);
    free(user->username);
    free(user);
}

void initListUser(int size)
{
    if (!hash)
        hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, _freeUser);
}

int loadUser(char *sp)
{
    User *user = (User *)malloc(sizeof(User));
    char *aux;
    user->username = strdup(strsep(&sp, ";"));
    if (strlen(user->username) == 0)
    {
        free(user->username);
        free(user);
        return 0;
    }
    user->name = strdup(strsep(&sp, ";"));
    if (strlen(user->name) == 0)
    {
        free(user->username);
        free(user->name);
        free(user);
        return 0;
    }
    user->gender = strsep(&sp, ";")[0];
    if (user->gender == '\0')
    {
        free(user->username);
        free(user->name);
        free(user);
        return 0;
    }
    aux = strdup(strsep(&sp, ";"));
    user->birthdate = sToDate(aux, strlen(aux));
    if (!user->birthdate)
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
    if (!user->account_creation)
    {
        free(user->username);
        free(user->name);
        free(user->birthdate);
        free(user);
        free(aux);
        return 0;
    }
    free(aux);
    aux = strdup(strsep(&sp, ";"));
    if (strlen(aux) == 0)
    {
        free(user->username);
        free(user->name);
        free(user->birthdate);
        free(user->account_creation);
        free(aux);
        free(user);
        return 0;
    }
    free(aux);
    aux = strdup(strsep(&sp, "\n"));
    for (int i = 0; i < strlen(aux); i++)
    {
        aux[i] = tolower(aux[i]);
    }
    if (strcmp(aux, "active") == 0)
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
        free(user);
        return 0;
    }
    user->age = calculateAge(user->birthdate);
    user->account_age = calculateAge(user->account_creation);
    user->avgScore = -1;
    user->totalDist = 0;
    user->recentRide = NULL;
    g_hash_table_insert(hash, user->username, user);
    free(aux);
    return 1;
}

User *findUserByUsername(char *username)
{
    return (User *)g_hash_table_lookup(hash, username);
}

bool isUserActive(char *username)
{
    //printf("%s\n", username);
    return findUserByUsername(username)->account_status;
}

char *getUserBasicInfo(char *id)
{
    User *user = findUserByUsername(id);
    char aux[10];
    char *r = (char *)malloc(256);
    strcpy(r, user->name);
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

void freeUser()
{
    g_hash_table_destroy(hash);
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
    User *a = *(User **)A;
    User *b = *(User **)B;

    if (a->totalDist == 0 && isUserActive(a->username) == true)
        a->totalDist = calculateUserTotalDist(a->username);
    if (b->totalDist == 0 && isUserActive(b->username) == true)
        b->totalDist = calculateUserTotalDist(b->username);

    if (a->totalDist < b->totalDist)
        return -1;
    if (a->totalDist == b->totalDist)
        return mostRecentRide(a->username, b->username) * -1;
    return 1;
}

gint gCompareUsersByDistance(gconstpointer A, gconstpointer B)
{
    User *a = (User *)A;
    User *b = (User *)B;

    if (a->totalDist == 0 && isUserActive(a->username) == true)
        a->totalDist = calculateUserTotalDist(a->username);
    if (b->totalDist == 0 && isUserActive(b->username) == true)
        b->totalDist = calculateUserTotalDist(b->username);

    if (a->totalDist < b->totalDist)
        return 1;
    if (a->totalDist == b->totalDist)
        return mostRecentRide(a->username, b->username);
    return -1;
}

char **mostDistUsers(int n)
{
    GList *glist = g_hash_table_get_values(hash);
    glist = g_list_sort(glist, gCompareUsersByDistance);
    char **r = malloc(sizeof(char *) * n);
    char aux[10];
    User *user;
    GList *l;
    int i = 0;
    for (l = glist; l && i < n; l = l->next)
    {
        user = (User *)l->data;
        r[i] = (char *)malloc(strlen(user->username) + strlen(user->name) + 15);
        sprintf(aux, "%d", user->totalDist);
        strcpy(r[i], user->username);
        strcat(r[i], ";");
        strcat(r[i], user->name);
        strcat(r[i], ";");
        strcat(r[i], aux);
        i++;
    }
    g_list_free(glist);
    return r;
}

gboolean doesUserExist(char *username)
{
    return g_hash_table_contains(hash, username);
}

Date *getUserRecentDate(char *username)
{
    return findUserByUsername(username)->recentRide;
}

void setUserRecentDate(char *username, Date *date)
{
    User *user = findUserByUsername(username);
    user->recentRide = date;
}

char getUserGender(char *username)
{
    return findUserByUsername(username)->gender;
}

char *getUserUsernameAndName(char *username)
{
    User *user = findUserByUsername(username);
    char *r = (char *)malloc(256);
    strcpy(r, user->username);
    strcat(r, ";");
    strcat(r, user->name);
    return r;
}

Date *getUserAccCreation(char *username)
{
    return findUserByUsername(username)->account_creation;
}