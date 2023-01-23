#include "Queries.h"
#include "Driver.h"
#include "Ride.h"
#include "User.h"
#include <stdlib.h>
#include <string.h>
#include "FileManager.h"
#include "ArrayList.h"
#include <math.h>

typedef struct query
{
    char id;
    char **args;
} Query;

static ArrayList *list = NULL;
int remainingQueries[9] = {0,0,0,0,0,0,0,0,0};

void initListQuery(int size)
{
    if (!list)
        list = createAL(size, sizeof(Query *));
}

int getNArgs(char id)
{
    switch (id)
    {
    case '1':
    case '2':
    case '3':
    case '4':
        return 1;
    case '5':
    case '7':
    case '8':
    case '9':
        return 2;

    case '6':
        return 3;
    }
    return 0;
}

int loadQuery(char *sp)
{
    if(strcmp(sp, "\n") == 0)
    {
        return 0;
    }
    Query *query = (Query *)malloc(sizeof(Query));
    query->id = strsep(&sp, " ")[0];
    int aux = query->id - '0';
    remainingQueries[aux - 1]++;
    int nArgs = getNArgs(query->id);
    query->args = malloc(sizeof(char *) * nArgs);
    for (int i = 0; i < nArgs; i++)
    {
        if (i == nArgs - 1)
        {
            query->args[i] = strdup(strsep(&sp, "\n"));
            break;
        }
        query->args[i] = strdup(strsep(&sp, " "));
    }
    addAL(list, query);
    return 1;
}

char *query1_drivers(char *id)
{
    if (isDriverActive(id) == false)
        return "";

    char *r = getDriverBasicInfo(id);

    if (doesDriverHaveRides(id) == FALSE)
    {
        strcat(r, "0.000;0;0.000");
        return r;
    }

    double *values = getDriverAvgScoreAndPay(id);
    char aux[100];
    sprintf(aux, "%.3f", values[0]);
    strcat(r, aux);
    strcat(r, ";");
    sprintf(aux, "%d", getNumberOfRidesDriver(id));
    strcat(r, aux);
    strcat(r, ";");
    sprintf(aux, "%.3f", values[1]);
    strcat(r, aux);
    free(values);
    return r;
}

char *query1_users(char *id)
{

    if (isUserActive(id) == false)
        return "";

    char *r = getUserBasicInfo(id);

    if (doesUserHaveRides(id) == FALSE)
    {
        strcat(r, "0,000;0;0.000");
        return r;
    }

    double *values = getUserAvgScoreAndPay(id);
    char aux[100];
    sprintf(aux, "%.3f", values[0]);
    strcat(r, aux);
    strcat(r, ";");
    sprintf(aux, "%d", getNumberOfRidesUser(id));
    strcat(r, aux);
    strcat(r, ";");
    sprintf(aux, "%.3f", values[1]);
    strcat(r, aux);
    free(values);
    return r;
}

char *query1(char *id)
{
    if (atoi(id) != 0 && doesDriverExist(id) == true)
        return query1_drivers(id);
    if (doesUserExist(id) == TRUE)
        return query1_users(id);
    return "";
}

LinkedList *query2(int n)
{
    if (n == 0)
        return NULL;
    char **r = topNdrivers(n);
    LinkedList *l = createLL();
    addLL(l, r);
    int *s = (int *)malloc(sizeof(int));
    *s = n;
    addLL(l, s);
    return l;
}

LinkedList *query3(int n)
{
    if (n == 0)
        return NULL;
    char **r = mostDistUsers(n);
    LinkedList *l = createLL();
    addLL(l, r);
    int *s = (int *)malloc(sizeof(int));
    *s = n;
    addLL(l, s);
    return l;
}

double query4(char *city)
{
    return avgPayInCity(city);
}

double query5(char *dataA, char *dataB) 
{
    return avgPayByDate(dataA, dataB);
}

double query6(char *city, char *dataA, char *dataB)
{

    return avgDistanceInCityByDate(city, dataA, dataB);
}

LinkedList *query7(int n, char *city)
{
    char **r = driversByScoreInCity(city, n);
    if (!r)
        return NULL;
    LinkedList *l = createLL();
    addLL(l, r);
    int *s = (int *)malloc(sizeof(int));
    *s = n;
    addLL(l, s);
    return l;
}

LinkedList *query8(char gender, int years)
{
    return ridesWithSameGenderAndAccAge(gender, years);
}

LinkedList *query9(char *dataA, char *dataB)
{
    return ridesWithTipByDistance(dataA, dataB);
}

void checkUselessHashs()
{
    if(remainingQueries[0] == 0 && remainingQueries[1] == 0)
        freeDriverHash();
    if(remainingQueries[0] == 0 && remainingQueries[2] == 0)
        freeUserHash();
    if(remainingQueries[3] == 0 && remainingQueries[5] == 0 && remainingQueries[6] == 0)
        freeCityHash();
    if(remainingQueries[6] == 0)
        freeScoreHash();
    if(remainingQueries[7] == 0)
        freeAccAgeHash();
}

void executeQueries()
{
    int nQueries = getALSize(list);
    char aux[100];
    char *aux2;
    Query *query;
    for (int i = 0; i < nQueries; i++)
    {
        query = (Query *)getByIndex(list, i);
        if(!query)
            return;
        switch (query->id)
        {
        case '1':
            aux2 = query1(query->args[0]);
            output(aux2, i);
            free(query->args[0]);
            if (strcmp(aux2, "") != 0)
                free(aux2);
            break;
        case '2':
            outputMult(query2(atoi(query->args[0])), i);
            free(query->args[0]);
            break;
        case '3':
            outputMult(query3(atoi(query->args[0])), i);
            free(query->args[0]);
            break;
        case '4':
            sprintf(aux, "%.3lf", query4(query->args[0]));
            output(aux, i);
            free(query->args[0]);
            break;
        case '5':
            sprintf(aux, "%.3lf", query5(query->args[0], query->args[1]));
            output(aux, i);
            free(query->args[0]);
            free(query->args[1]);
            break;
        case '6':
            sprintf(aux, "%.3lf", query6(query->args[0], query->args[1], query->args[2]));
            output(aux, i);
            free(query->args[0]);
            free(query->args[1]);
            free(query->args[2]);
            break;
        case '7':
            outputMult(query7(atoi(query->args[0]), query->args[1]), i);
            free(query->args[0]);
            free(query->args[1]);
            break;
        case '8':
            outputMult(query8(query->args[0][0], atoi(query->args[1])), i);
            free(query->args[0]);
            free(query->args[1]);
            break;
        case '9':
            outputMult(query9(query->args[0], query->args[1]), i);
            free(query->args[0]);
            free(query->args[1]);
            break;            
        default:
            break;
        }
        int aux = query->id - '0';
        remainingQueries[aux - 1]--;
        checkUselessHashs();
        free(query->args);
        free(query);
    }
}

void freeQuery()
{
    freeArrayListSimple(list);
}

void printQueries(LinkedList *l, bool isPaged, char *firstLine)
{
    if (!l) return;
    char **r = (char **)iterateLL(l);
    int *s = (int *)iterateLL(l);
    printf("%s\n", firstLine);
    if (!isPaged)
    {
        for (int i = 0; i < *s; i++)
        {
            printf("%s\n", r[i]);
        }
    }
    else
    {
        int perPage = 51, i, start = 0, page = 1;
        char input;
        while (page != -1)
        {
            printf("%s\n", firstLine);
            for (i = start; i < perPage; i++)
            {
                printf("%s\n", r[i]);
            }
            printf("Page %d       (a <- -> d   q-quit)     ", page);
            input = getchar();
            getchar(); // BUFFER
            system("clear");
            switch (input)
            {
            case 'd':
                if (i != *s)
                {
                    page++;
                    start = perPage;
                    perPage += 51;
                    if (perPage > *s)
                        perPage = *s;
                }
                break;
            case 'a':
                if (page != 1)
                {
                    perPage = start;
                    start -= 51;
                    if (start < 0)
                        start = 0;
                    page--;
                }
                break;
            case 'q':
                page = -1;
                break;
            default:
                break;
            }
        }
    }
    free(r);
    free(s);
    freeLinkedList(l);
}

void executeQuery(char id, char **args)
{
    system("clear");
    bool page;
    int aux;
    switch (id)
    {
    case '1':
        printf("%s\n", query1(args[0]));
        break;
    case '2':
        aux = atoi(args[0]);
        if (aux > 51)
            page = true;
        else
            page = false;
        printQueries(query2(atoi(args[0])), page, "id;nome;avaliacao_media");
        break;
    case '3':
        aux = atoi(args[0]);
        if (aux > 51)
            page = true;
        else
            page = false;
        printQueries(query3(atoi(args[0])), page, "username;nome;distancia_total");
        break;
    case '4':
        printf("%.3lf\n", query4(args[0]));
        break;
    case '5':
        printf("%.3lf\n", query5(args[0], args[1]));
        break;
    case '6':
        printf("%.3lf\n", query6(args[0], args[1], args[2]));
        break;
    case '7':
        aux = atoi(args[0]);
        if (aux > 51)
            page = true;
        else
            page = false;
        printQueries(query7(atoi(args[0]), args[1]), page, "id;nome;avaliacao_media");
        break;
    case '8':
        printQueries(query8(args[0][0], atoi(args[1])), true, "id_condutor;nome_condutor;username_utilizador;nome_utilizador");
        break;
    case '9':
        printQueries(query9(args[0], args[1]), true, "id_viagem;data_viagem;distancia;cidade;valor_gorjeta");
        break;    
    default:
        break;
    }
}

void executeTests(char const *argv)
{
    int nQueries = getALSize(list);
    char aux[100];
    char *aux2, *verificacao;
    Query *query;
    clock_t start = 0, end = 0;
    double cpu_time_used = 0, segundos = 0;
    
    for (int i = 0; i < nQueries; i++)
    {
        query = (Query *)getByIndex(list, i);
        start = clock();
        switch (query->id)
        {
        case '1':
            aux2 = query1(query->args[0]);
            output(aux2, i);
            free(query->args[0]);
            if (strcmp(aux2, "") != 0)
                free(aux2);
            break;
        case '2':
            outputMult(query2(atoi(query->args[0])), i);
            free(query->args[0]);
            break;
        case '3':
            outputMult(query3(atoi(query->args[0])), i);
            free(query->args[0]);
            break;
        case '4':
            sprintf(aux, "%.3lf", query4(query->args[0]));
            output(aux, i);
            free(query->args[0]);
            break;
        case '5':
            sprintf(aux, "%.3lf", query5(query->args[0], query->args[1]));
            output(aux, i);
            free(query->args[0]);
            free(query->args[1]);
            break;
        case '6':
            sprintf(aux, "%.3lf", query6(query->args[0], query->args[1], query->args[2]));
            output(aux, i);
            free(query->args[0]);
            free(query->args[1]);
            free(query->args[2]);
            break;
        case '7':
            outputMult(query7(atoi(query->args[0]), query->args[1]), i);
            free(query->args[0]);
            free(query->args[1]);
            break;
        case '8':
            outputMult(query8(query->args[0][0], atoi(query->args[1])), i);
            free(query->args[0]);
            free(query->args[1]);
            break;
        case '9':
            outputMult(query9(query->args[0], query->args[1]), i);
            free(query->args[0]);
            free(query->args[1]);
            break;            
        default:
            break;
        }

        end = clock();
        cpu_time_used = ((double) (end - start));
        segundos = cpu_time_used / CLOCKS_PER_SEC;
        verificacao = (char *)malloc(256);
        if (compareFiles(argv, i)) {
            strcpy(verificacao, "Correto");
        } else strcpy(verificacao, "Incorreto");

        printf("Comando %d (Q%c):\nDuracao (s):%lf\nResultado:%s\n\n", (i+1), query->id, segundos, verificacao);

        free(query->args);
        free(query);
        free(verificacao);
    }
}
