#ifndef DOOR_H
#define DOOR_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <sstream>

using namespace std;

class Door
{

public:
    Door(int doorID);
    ~Door();

    void openorclose(bool doorState);

private:
    int doorID;
    bool doorState;
};

#endif // DOOR_H
