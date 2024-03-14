#include "elevator.h"

Elevator::Elevator(int elevatorID, int numFloors) {
    this->elevatorID = elevatorID;
    this->direction = "";
    this->doorOpen = false;
    this->overloadStatus = false;
    this->emergencyStatus = false;
    this->currFloor = 0;
    this->numFloors = numFloors;
    this->doorTimer = 0;
}

Elevator::~Elevator() {
}


int Elevator::getElevatorID() {
    return this->elevatorID;
}

QVector<Request*>& Elevator::getRequests() {
    return this->requests;
}


QString Elevator::getDirection() {
    return this->direction;
}

bool Elevator::isDoorOpen() {
    return this->doorOpen;
}

bool Elevator::getOverloadStatus() {
    return this->overloadStatus;
}

bool Elevator::getEmergencyStatus() {
    return this->emergencyStatus;
}

bool Elevator::getObstacleStatus() {
    return this->obstacleStatus;
}

bool Elevator::getOutageStatus() {
    return this->outageStatus;
}

bool Elevator::getFireStatus() {
    return this->fireStatus;
}

int Elevator::getCurrFloor() {
    return this->currFloor;
}

int Elevator::getDoorTimer() {
    return this->doorTimer;
}

void Elevator::setElevatorID(int elevatorID) {
    this->elevatorID = elevatorID;
}

void Elevator::setRequests(QVector<Request *> requests) {
    this->requests = requests;
}


void Elevator::setDirection(QString direction) {
    this->direction = direction;
}

void Elevator::setDoorOpen(bool doorOpen) {
    this->doorOpen = doorOpen;
}

void Elevator::setOverloadStatus(bool overloadStatus) {
    this->overloadStatus = overloadStatus;
}

void Elevator::setEmergencyStatus(bool emergencyStatus) {
    this->emergencyStatus = emergencyStatus;
}

void Elevator::setObstacleStatus(bool obstacleStatus) {
    this->obstacleStatus = obstacleStatus;
}

void Elevator::setFireStatus(bool fireStatus) {
    this->fireStatus = fireStatus;
}

void Elevator::setOutageStatus(bool outageStatus) {
    this->outageStatus = outageStatus;
}

void Elevator::setCurrFloor(int currFloor) {
    this->currFloor = currFloor;
}

void Elevator::setDoorTimer(int doorTimer) {
    this->doorTimer = doorTimer;
}

void Elevator::move() {
    decide();

    if (direction.isEmpty() && requests.empty()) {
        if(doorOpen){
            emit printToConsole("ELEVATOR #" + QString::number(elevatorID) + " DOOR CLOSED \n");
            setDoorOpen(false);
        }
        return;
    }

    if (!doorOpen) {
        for (auto it = requests.begin(); it != requests.end();) {
            if (currFloor == (*it)->getDestFloor()) {
                emit printToConsole("*RING* ELEVATOR #" + QString::number(elevatorID) + " DOOR OPENED AT FLOOR#" + QString::number(currFloor) + "\n");
                if ((*it)->getSource() == Request::ELEVATOR) {
                    emit printToConsole("ELEVATOR #" + QString::number(elevatorID) + " WAITING FOR PASSENGERS TO LEAVE\n");
                } else {
                    emit printToConsole("ELEVATOR #" + QString::number(elevatorID) + " WAITING FOR PASSENGERS TO EMBARK\n");
                }
                if(fireStatus){
                    emit printToConsole("*RING* FIRE PROTOCOL:: ELEVATOR #" + QString::number(elevatorID) + " PASSENGERS LEAVE NOW\n");
                    setFireStatus(false);
                }
                if(outageStatus){
                    emit printToConsole("*RING* sOUTAGE PROTOCOL:: ELEVATOR #" + QString::number(elevatorID) + " PASSENGERS LEAVE NOW\n");
                    setOutageStatus(false);
                }
                stop();
                delete (*it);
                requests.erase(it);
                return;
            } else {
                ++it;
            }
        }

        if(overloadStatus || obstacleStatus){
            return;
        }
        if (direction == "UP" && !doorOpen) {
            setCurrFloor(getCurrFloor() + 1);
            emit printToConsole("ELEVATOR #" + QString::number(elevatorID) + " IS AT FLOOR#" + QString::number(getCurrFloor()) + "\n");
        } else if (direction == "DOWN" && !doorOpen) {
            setCurrFloor(getCurrFloor() - 1);
            emit printToConsole("ELEVATOR #" + QString::number(elevatorID) + " IS AT FLOOR#" + QString::number(getCurrFloor()) + "\n");
        }
        return;
    }

    if (doorOpen) {
        doorTimer++;
        if (doorTimer == 1) {
            setDoorOpen(false);
            doorTimer = 0;
            emit printToConsole("*RING* ELEVATOR #" + QString::number(elevatorID) + " DOOR CLOSED AT FLOOR#" + QString::number(currFloor) + "\n");
        }
        return;
    }

    if (requests.empty()) {
        setDirection("");
        return;
    }
}

void Elevator::stop() {
       setDoorOpen(true);
}

void Elevator::decide() {
       if (requests.empty()) {
           setDirection("");
           return;
       }

       if (getCurrFloor() == 0 && !requests.empty()) {
           setDirection("UP");
           return;
       } else if ((getCurrFloor() == numFloors - 1 && !requests.empty())) {
           setDirection("DOWN");
           return;
       }

       int firstItemDiff = requests[0]->getDestFloor() - getCurrFloor();

       for (int i = 1; i < requests.size(); ++i) {
           int diff = requests[i]->getDestFloor() - getCurrFloor();
           if ((diff > 0 && firstItemDiff < 0) || (diff < 0 && firstItemDiff > 0)) {
               firstItemDiff = diff;
           }
       }

       if (firstItemDiff > 0) {
           setDirection("UP");
       } else if (firstItemDiff < 0) {
           setDirection("DOWN");
       } else {
           setDirection("");
       }
}
