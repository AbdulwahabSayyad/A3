#include "request.h"

Request::Request() {
    // Constructor implementation
}

Request::Request(int destFloor, QString direction) {
    // Constructor implementation
    this->source = Request::FLOOR;
    this->destFloor = destFloor;
    this->direction = direction;
    this->elevatorID = -1;
    this->status = Request::PENDING;
}

Request::Request(int elevatorID, int destFloor) {
    // Constructor implementation
    this->source = Request::ELEVATOR;
    this->destFloor = destFloor;
    this->direction = "";
    this->elevatorID = elevatorID;
    this->status = Request::TAKEN;
}

Request::~Request() {
    // Destructor implementation
}

int Request::getDestFloor(){
    return this->destFloor;
}

QString Request::getDirection(){
    return this->direction;
}

int Request::getElevatorID(){
    return this->elevatorID;
}

void Request::setDestFloor(int floor){
    this->destFloor = floor;
}

void Request::setDirection(QString direction){
    this->direction = direction;
}

void Request::setElevatorID(int elevatorID){
    this->elevatorID = elevatorID;
}

QString Request::sourceToString(Source source) {
    switch (source) {
        case ELEVATOR: return "ELEVATOR";
        case FLOOR: return "FLOOR";
        default: return "UNKNOWN";
    }
}

Request::Source Request::stringToSource(const QString& str) {
    if (str == "ELEVATOR") return ELEVATOR;
    if (str == "FLOOR") return FLOOR;
    return ELEVATOR;
}

QString Request::statusToString(Status status) {
    switch (status) {
        case PENDING: return "PENDING";
        case TAKEN: return "TAKEN";
        case SERVICED: return "SERVICED";
        default: return "UNKNOWN";
    }
}

Request::Status Request::stringToStatus(const QString& str) {
    if (str == "PENDING") return PENDING;
    if (str == "TAKEN") return TAKEN;
    if (str == "SERVICED") return SERVICED;
    return PENDING;
}

Request::Source Request::getSource(){
    return this->source;
}

Request::Status Request::getStatus(){
    return this->status;
}

void Request::setSource(Request::Source source){
    this->source = source;
}

void Request::setStatus(Request::Status status){
    this->status = status;
}

