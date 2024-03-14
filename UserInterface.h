#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "ECS.h"
#include "Floor.h"

#include <QVector>

using namespace std;

class UserInterface
{

public:
    UserInterface(ECS *ecs);
    ~UserInterface();

    void callElevatorUp(int floor);
    void callElevatorDown(int floor);
    void startFireAlarm(int floor);
    void startFireAlarm(Elevator *elevator);
    void startPowerOutage();

private:
    ECS *ecs;
    QVector<Floor*> floors;
};

#endif // USERINTERFACE_H
