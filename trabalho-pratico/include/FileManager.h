#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

    #include "ArrayList.h"
    #include <stdio.h>
    
    ArrayList* load(char*, int (*getElementSizeFunc)(), void* (*loadFunc)(char*));
    int getLines(FILE* file);

#endif