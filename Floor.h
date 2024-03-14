#ifndef FLOOR_H
#define FLOOR_H

#include "Button.h"

using namespace std;

class Floor
{

public:
    Floor(int floorID);
    ~Floor();

private:
    int floorID;
    Button *upButton;
    Button *downButton;
};

#endif // FLOOR_H
