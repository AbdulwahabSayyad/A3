#ifndef SENSOR_H
#define SENSOR_H

#include <QString>

using namespace std;

class Sensor
{

public:
    Sensor(int sensorID, QString sensorType);
    ~Sensor();

    void activateSensor(QString sensorType);

private:
    int sensorID;
    QString sensorType;
};

#endif // SENSOR_H

