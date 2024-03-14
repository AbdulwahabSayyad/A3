#ifndef ECS_H
#define ECS_H

#include "elevator.h"
#include "request.h"
#include <limits>
#include <QObject>
#include <QVector>

using namespace std;

class ECS : public QObject
{
    Q_OBJECT

public:
    ECS();
    ECS(int,int,int);
    ~ECS();

    int selectBestElevator(Request* req);
    int calculateDistance(int reqFloor, QString direction, Elevator* elevator);
    int determineSafeFloor();
    void fireProtocol();
    void helpProtocol(Elevator* elevator);
    void callEmergencies(Elevator* elevator);
    void overloadProtocol(Elevator* elevator);
    void obstacleProtocol(Elevator* elevator);
    void outageProtocol();
    int getNumElevators();
    int getNumFloors();
    void next();
    void printState();

    QVector<Elevator*>& getElevators();
    QVector<Request*>& getRequests();
    QVector<int> getFloors();
    int getMaxWeight();

    void setElevators(QVector<Elevator*> elevators);
    void setRequests(QVector<Request*> requests);
    void setFloors(QVector<int> floors);
    void setMaxWeight(int maxWeight);

signals:
    void printToConsole(QString text);

private:
    QVector<Elevator*> elevators;
    QVector<Request*> requests;
    QVector<int> floors;
    int maxWeight;
};

#endif // ECS_H
