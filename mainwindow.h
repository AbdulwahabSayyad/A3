#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ecs.h"
#include "QTimer"
#include "request.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void delay(int millisecond);
    void startSimulation();
    void init(int numFloors, int numElevators, int maxWeight);
    void quitSimulation();
    void fpSubmitRequest();
    void epSubmitRequest();
    void sfSubmitRequest();
    void proc();

public slots:
    void printFromClass(QString text);

private:
    Ui::MainWindow *ui;
    ECS* ecs;
    int numFloors;
    int numElevators;
    int maxWeight;
    bool procTimer;

    void onfpIndexChanged();
    void onsfButtonClicked();
    void onepElevatorChanged();
    void onfpButtonClicked();
    void onProcClicked();
};
#endif // MAINWINDOW_H
