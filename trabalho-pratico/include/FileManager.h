#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <stdio.h>

void load(char *, void *(*loadFunc)(char *), void (*initFunc)(int), int);
int getLines(FILE *);
void output(char *, int);

#endif
