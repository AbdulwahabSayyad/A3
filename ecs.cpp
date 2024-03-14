#include "ecs.h"

ECS::ECS(){

}

ECS::ECS(int numElevators, int numFloors, int maxWeight) {

        for (int i = 0; i < numFloors; ++i) {
            floors.push_back(i);
        }

        for (int i = 0; i < numElevators; ++i) {
            elevators.push_back(new Elevator(i, this->getNumFloors()));
        }

        requests = QVector<Request*>();

        this->maxWeight = maxWeight;
}

ECS::~ECS() {
}

int ECS::getNumFloors(){
    return this->floors.length();
}

int ECS::getNumElevators(){
    return this->elevators.length();
}

QVector<Elevator*>& ECS::getElevators(){
    return this->elevators;
}

QVector<Request*>& ECS::getRequests(){
    return this->requests;
}

QVector<int> ECS::getFloors(){
    return this->floors;
}

int ECS::getMaxWeight(){
    return this->maxWeight;
}

void ECS::setElevators(QVector<Elevator *> elevators){
    this->elevators = elevators;
}

void ECS::setRequests(QVector<Request *> requests){
    this->requests = requests;
}

void ECS::setFloors(QVector<int> floors){
    this->floors = floors;
}

void ECS::setMaxWeight(int maxWeight){
    this->maxWeight = maxWeight;
}

int ECS::selectBestElevator(Request* req) {
    int requestedFloor = req->getDestFloor();
    QString requestedDirection = req->getDirection();
    int distance = -1;
    int bestDistance = std::numeric_limits<int>::max();
    int bestElevator = 0;

    for (Elevator *elevator : this->getElevators()) {
        if(elevator->getCurrFloor() == requestedFloor && elevator->isDoorOpen() && !elevator->getOverloadStatus() && !elevator->getEmergencyStatus()){
            return elevator->getElevatorID();
        }
    }

    for (Elevator *elevator : this->getElevators()) {
        distance = this->calculateDistance(requestedFloor, requestedDirection, elevator);
        if (!elevator->getOverloadStatus() && !elevator->getEmergencyStatus()) {
            if (distance < bestDistance) {
                bestDistance = distance;
                bestElevator = elevator->getElevatorID();
            }
         }
       }

    return bestElevator;
}

int ECS::calculateDistance(int reqFloor, QString direction, Elevator* elevator){
    int distance = std::numeric_limits<int>::max();
    int topFloor = this->getNumFloors()-1;

    if(direction == "UP" ){
        if(elevator->getDirection()=="UP"){
            if(elevator->getCurrFloor()<reqFloor){
                distance = reqFloor - elevator->getCurrFloor();
            }
            else if(elevator->getCurrFloor()>reqFloor){
                distance = (topFloor-elevator->getCurrFloor()) + (topFloor-reqFloor);
            }
        }
        else if(elevator->getDirection()=="DOWN"){
            distance = elevator->getCurrFloor() + reqFloor;
        }
        else if(elevator->getDirection()==""){
            distance = abs(elevator->getCurrFloor()-reqFloor);
        }
    }
    else if(direction == "DOWN"){
        if(elevator->getDirection()=="UP"){
            distance = (topFloor-elevator->getCurrFloor()) + (topFloor-reqFloor);
        }
        else if(elevator->getDirection()=="DOWN"){
            if(elevator->getCurrFloor()>reqFloor){
                distance = elevator->getCurrFloor() - reqFloor;
            }
            else if(elevator->getCurrFloor()<reqFloor){
                distance = elevator->getCurrFloor() + reqFloor;
            }
        }
        else if(elevator->getDirection()==""){
            distance = abs(elevator->getCurrFloor()-reqFloor);
        }
    }
    return distance;
}

int ECS::determineSafeFloor() {
    return 0;
}

void ECS::fireProtocol() {
    int safeFloor = determineSafeFloor();
    emit printToConsole("SAFE FLOOR = " + QString::number(safeFloor) + "\n");
    for(Elevator* elevator : getElevators()){
        elevator->setFireStatus(true);
        elevator->setDoorOpen(false);
        elevator->getRequests().clear();
        Request* req = new Request(elevator->getElevatorID(), safeFloor);
        elevator->getRequests().append(req);
        emit printToConsole("ELEVATOR " + QString::number(elevator->getElevatorID()) + " MOVING TO SAFE FLOOR " + QString::number(safeFloor) +"\n");
    }
}

void ECS::helpProtocol(Elevator* elevator) {
    elevator->setEmergencyStatus(true);
    emit printToConsole("EMERGENCY: ELEVATOR " + QString::number(elevator->getElevatorID()) + " HAS AN EMERGENCY. CONNECTING TO MANAGEMENT\n");
    emit printToConsole("EMERGENCY: ELEVATOR " + QString::number(elevator->getElevatorID()) + " MIC AND SPEAKER ON\n");
}

void ECS::callEmergencies(Elevator* elevator) {
    emit printToConsole("EMERGENCY: ELEVATOR " + QString::number(elevator->getElevatorID()) + " STAFF NOT RESPONDING. CONNECTING TO EMERGENCY LINE\n");
}

void ECS::overloadProtocol(Elevator* elevator) {
    emit printToConsole("OVERLOAD: ELEVATOR " + QString::number(elevator->getElevatorID()) + " HAS WEIGHT OVER "+QString::number(maxWeight)+ ". PLEASE DECREASE LOAD\n");
    elevator->setOverloadStatus(true);
}

void ECS::obstacleProtocol(Elevator* elevator) {
    emit printToConsole("OBSTACLE: ELEVATOR " + QString::number(elevator->getElevatorID()) + " HAS AN OBSTACLE. BEEP BEEP\n");
    elevator->setObstacleStatus(true);
}

void ECS::outageProtocol() {
    int safeFloor = determineSafeFloor();
    emit printToConsole("SAFE FLOOR = " + QString::number(safeFloor) + "\n");
    for(Elevator* elevator : getElevators()){
        elevator->setOutageStatus(true);
        elevator->setDoorOpen(false);
        elevator->getRequests().clear();
        Request* req = new Request(elevator->getElevatorID(), safeFloor);
        elevator->getRequests().append(req);
        emit printToConsole("ELEVATOR " + QString::number(elevator->getElevatorID()) + " MOVING TO SAFE FLOOR " + QString::number(safeFloor) +"\n");
    }
}

void ECS::next(){
    for(Elevator* elevator: getElevators()){
        elevator->move();
        if(elevator->getEmergencyStatus()){
            elevator->setEmergencyStatus(false);
            callEmergencies(elevator);
        }
        if(elevator->getOverloadStatus()){
            emit printToConsole("ELEVATOR "+QString::number(elevator->getElevatorID())+" LOAD DECREASED TO BELOW THRESHOLD.\n");
            elevator->setOverloadStatus(false);
        }
        if(elevator->getObstacleStatus()){
            elevator->setObstacleStatus(false);
        }
    }
}

void ECS::printState(){
    emit printToConsole("\n--------------\n");
    emit printToConsole("Current State:\n");
    for(Elevator* elevator: getElevators()){
        emit printToConsole("E #" + QString::number(elevator->getElevatorID()) + " | F #" +QString::number(elevator->getCurrFloor())+ "\n" );
    }
}
