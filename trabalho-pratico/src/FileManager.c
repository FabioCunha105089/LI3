#include "FileManager.h"
#include "Driver.h"
#include "Ride.h"
#include "User.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Date sToDate(char* string){
    Date date;
    date.day = atoi(strsep(&string, "/"));
    date.month = atoi(strsep(&string, "/"));
    date.year = atoi(strsep(&string, "\0"));
    return date;
}

int getLines(FILE* file){
    int size = 0;
    char c = fgetc(file);
    while(!feof(file)){
        if(c == '\n'){
            size++;
        }
        c = fgetc(file);
    }
    fseek(file, 0, SEEK_SET);
    return size;
}

Driver* loadDrivers(){
    char* path = "drivers.csv";
    char* line = (char*)malloc(sizeof(char) * 256);
    char* sp;
    char* token;
    FILE* file = fopen(path, "r");
    if(file == NULL){
        printf("Ficheiro não encontrado");
        return NULL;
    }
    int size = getLines(file) - 1;
    Driver* drivers = (Driver*)malloc(sizeof(Driver) * size);
    fgets(line, 256, file);
    for(int i = 0; i < size; i++){
        fgets(line, 256, file);
        sp = strdup(line);
        drivers[i].id = strsep(&sp, ";");
        drivers[i].name = strsep(&sp, ";");
        drivers[i].birth_day = sToDate(strsep(&sp, ";"));
        drivers[i].gender = strsep(&sp, ";")[0];
        drivers[i].car_class = strsep(&sp, ";");
        drivers[i].license_plate = strsep(&sp, ";");
        drivers[i].city = strsep(&sp, ";");
        drivers[i].account_creation = sToDate(strsep(&sp, ";"));
        drivers[i].account_status = strcmp(strsep(&sp, "\n"), "active") == 0 ? TRUE : FALSE;
    }
    fclose(file);
    return drivers;
}

int main(int argc, char const *argv[])
{   
    Driver* drivers = loadDrivers();  
    for(int i = 0; i < 10000; i++){
        if(drivers[i].account_status == TRUE){
            printf("active\n");
        } else {
            printf("inactive");
        }
    }
    return 0;
}