#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QString>
#include <QObject>
#include <QVector>
#include "request.h"
#include <algorithm>

using namespace std;

class Elevator : public QObject
{
    Q_OBJECT

public:
    Elevator(int elevatorID, int numFloors);
    ~Elevator();

    void move();
    void stop();
    void decide();

    int getElevatorID();
    QVector<Request*>& getRequests();
    QString getDirection();
    bool isDoorOpen();
    bool getOverloadStatus();
    bool getEmergencyStatus();
    bool getObstacleStatus();
    bool getFireStatus();
    bool getOutageStatus();
    int getCurrFloor();
    int getDoorTimer();
    int getNumFloors();

    void setElevatorID(int elevatorID);
    void setRequests(QVector<Request*> requests);
    void setDirection(QString direction);
    void setDoorOpen(bool doorOpen);
    void setOverloadStatus(bool overloadStatus);
    void setEmergencyStatus(bool emergencyStatus);
    void setObstacleStatus(bool obstacleStatus);
    void setOutageStatus(bool outageStatus);
    void setFireStatus(bool fireStatus);
    void setCurrFloor(int currFloor);
    void setDoorTimer(int doorTimer);
    void setNumFloors(int numFloors);

signals:
    void printToConsole(QString text);

private:
    int elevatorID;
    QVector<Request*> requests;
    QString direction;
    int currFloor;
    bool doorOpen;
    bool overloadStatus;
    bool emergencyStatus;
    bool obstacleStatus;
    bool fireStatus;
    bool outageStatus;
    int doorTimer;
    int numFloors;

};

#endif // ELEVATOR_H

