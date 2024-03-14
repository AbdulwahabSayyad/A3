#ifndef REQUEST_H
#define REQUEST_H

#include <QString>
#include <QObject>

using namespace std;

class Request : public QObject
{
    Q_OBJECT

public:
    Request();
    Request(int destFloor,QString direction);
    Request(int elevatorID, int destFloor);
    ~Request();

    enum Source { ELEVATOR, FLOOR };
    enum Status { PENDING, TAKEN, SERVICED };

    static QString sourceToString(Source source);
    static QString statusToString(Status status);

    static Source stringToSource(const QString& str);
    static Status stringToStatus(const QString& str);

    int getDestFloor();
    QString getDirection();
    int getElevatorID();
    Source getSource();
    Status getStatus();

    void setDestFloor(int destFloor);
    void setDirection(QString direction);
    void setElevatorID(int elevatorID);
    void setSource(Source source);
    void setStatus(Status status);

signals:
    void printToConsole(QString text);


private:
    int destFloor;
    QString direction;
    int elevatorID;
    Source source;
    Status status;

};

#endif // REQUEST_H
