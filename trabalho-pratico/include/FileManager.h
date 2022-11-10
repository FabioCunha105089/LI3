#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

    #include "Driver.h"
    #include "Ride.h"
    #include "User.h"
    #include <stdio.h>
    
    Driver* loadDrivers();
    User* loadUsers();
    Ride* loadRides();
    int getLines(FILE* file);

#endif