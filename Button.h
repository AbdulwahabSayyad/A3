#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <sstream>

using namespace std;

class Button
{

public:
    Button(int buttonID);
    ~Button();

    void illuminateButton(bool buttonLight);

private:
    int buttonID;
    bool light;
};

#endif // BUTTON_H
