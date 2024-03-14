#ifndef DISPLAY_H
#define DISPLAY_H

#include <QString>

using namespace std;

class Display
{
public:
    Display(int displayID);
    ~Display();

    void displayText(const QString &text);

private:
    int displayID;
};

#endif // DISPLAY_H
