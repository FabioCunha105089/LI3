#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <stdio.h>
#include "LinkedList.h"
#include <stdbool.h>

int load(char *, int (*loadFunc)(char *), void (*initFunc)(int), int);
int getLines(FILE *);
void output(char *, int);
void outputMult(LinkedList*, int);
bool compareFiles(char const *, int);

#endif