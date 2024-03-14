#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->console->insertPlainText("---REI Simulator---\n");
    ui->quitSimulation->setVisible(false);
    ui->epWidget->setDisabled(true);
    ui->fpWidget->setDisabled(true);
    ui->sfWidget->setDisabled(true);
    ui->proc->setVisible(false);
    ui->printState->setVisible(false);

    this->numElevators = 0;
    this->numFloors = 0;
    this->maxWeight = 0;

    connect(ui->apSubmit, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(ui->quitSimulation, &QPushButton::clicked, this, &MainWindow::quitSimulation);
    connect(ui->epSubmit, &QPushButton::clicked, this, &MainWindow::epSubmitRequest);
    connect(ui->fpSubmit, &QPushButton::clicked, this, &MainWindow::fpSubmitRequest);
    connect(ui->sfSubmit, &QPushButton::clicked, this, &MainWindow::sfSubmitRequest);

    connect(ui->fpFloor, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onfpIndexChanged);
    connect(ui->epElevator, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onepElevatorChanged);

    connect(ui->sfFire, &QPushButton::pressed, this, &MainWindow::onsfButtonClicked);
    connect(ui->sfObstacle, &QPushButton::pressed, this, &MainWindow::onsfButtonClicked);
    connect(ui->sfOverload, &QPushButton::pressed, this, &MainWindow::onsfButtonClicked);
    connect(ui->sfOutage, &QPushButton::pressed, this, &MainWindow::onsfButtonClicked);
    connect(ui->sfEmergency, &QPushButton::pressed, this, &MainWindow::onsfButtonClicked);

    connect(ui->fpUp, &QPushButton::pressed, this, &MainWindow::onfpButtonClicked);
    connect(ui->fpDown, &QPushButton::pressed, this, &MainWindow::onfpButtonClicked);

    connect(ui->proc, &QPushButton::clicked, this, &MainWindow::proc);
    connect(ui->printState, &QPushButton::pressed, this, &MainWindow::printState);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(int numFloors, int numElevators, int maxWeight){
    this->numElevators = numElevators;
    this->numFloors = numFloors;
    this->maxWeight = maxWeight;
    this->procTimer = ui->procTimerCheckbox->isChecked();

    ecs = new ECS(numElevators, numFloors, maxWeight);
    connect(ecs, &ECS::printToConsole, this, &MainWindow::printFromClass);

    for(Elevator* elevator: ecs->getElevators()){
        connect(elevator, &Elevator::printToConsole, this, &MainWindow::printFromClass);
    }
    ui->printState->setVisible(true);
}

void MainWindow::startSimulation(){
    if(ui->numElevators->value() <= 0 || ui->numFloors->value() <= 0 || ui->maxWeight->value() <= 0){
        ui->console->insertPlainText("Num Floors, Num Elevators and Max Weight must have a nonzero value\n");
        this->delay(1000);
        this->quitSimulation();
        return;
    }

    if(ui->procTimerCheckbox->isChecked()){
        ui->proc->setVisible(false);
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::onProcClicked);
        timer->start(1500);
    }
    else{
        ui->proc->setVisible(true);
    }

    ui->apWidget->setDisabled(true);
    //delay(1000);
    ui->console->clear();
    ui->console->insertPlainText("Initializing simulation...\n");
    //delay(1000);
    init(ui->numFloors->value(), ui->numElevators->value(), ui->maxWeight->value());
    ui->console->insertPlainText("Num. of Elevators: " + QString::number(ecs->getNumElevators()) + "\n");
    //delay(1000);
    ui->console->insertPlainText("Num. of Floors: " + QString::number(ecs->getNumFloors()) + "\n");
    //delay(1000);
    ui->console->insertPlainText("Max. Weight: " + QString::number(ecs->getMaxWeight()) + "\n");

    for (int i = 0; i<this->numElevators; i++) {
        ui->epElevator->addItem(QString::number(ecs->getElevators()[i]->getElevatorID()));
        ui->sfElevator->addItem(QString::number(ecs->getElevators()[i]->getElevatorID()));
    }

    for (int i = 0; i<this->numFloors; i++) {
        ui->fpFloor->addItem(QString::number(ecs->getFloors()[i]));
    }

    ui->epWidget->setEnabled(true);
    ui->fpWidget->setEnabled(true);
    ui->sfWidget->setEnabled(true);
    ui->sfElevator->setDisabled(true);
    ui->quitSimulation->setVisible(true);


}


void MainWindow::quitSimulation(){
    delete ecs;
    ecs = nullptr;
    ui->console->clear();
    ui->console->insertPlainText("---REI Simulator---\n");
    ui->quitSimulation->setVisible(false);
    ui->apWidget->setEnabled(true);
    ui->epWidget->setDisabled(true);
    ui->fpWidget->setDisabled(true);
    ui->sfWidget->setDisabled(true);
    ui->proc->setVisible(false);
    ui->printState->setVisible(false);

    this->numElevators = 0;
    this->numFloors = 0;
    this->maxWeight = 0;
    this->procTimer = false;

    ui->epElevator->clear();
    ui->sfElevator->clear();
    ui->fpFloor->clear();
    ui->epDest->clear();
}


void MainWindow::delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

void MainWindow::fpSubmitRequest(){
    int sourceFloor = ui->fpFloor->currentIndex();
    QString direction = "";
    if(ui->fpDown->isChecked()){
        direction = ui->fpDown->text();
    }
    else if(ui->fpUp->isChecked()){
        direction = ui->fpUp->text();
    }
    else{
        ui->console->insertPlainText("Please select the direction before submitting the request\n");
        return;
    }

    for (Request *request : ecs->getRequests()) {
                if (request->getDestFloor() == sourceFloor && request->getSource()==Request::FLOOR && request->getDirection()==direction && (request->getStatus()==Request::PENDING || request->getStatus()==Request::TAKEN)) {
                    ui->console->insertPlainText("There is a pending/currently taken request from this Floor # " + QString::number(request->getDestFloor()) + " going " + direction +"\n");
                    ui->fpUp->setChecked(false);
                    ui->fpDown->setChecked(false);
                    return;
                }
            }

    Request* req = new Request(sourceFloor,direction);
    ecs->getRequests().append(req);
    ui->console->insertPlainText("Request from floor # " + QString::number(req->getDestFloor()) + " going " + req->getDirection() + " has been initiated\n");

    int bestElevator = ecs->selectBestElevator(req);
    ecs->getElevators()[bestElevator]->getRequests().append(req);
    req->setStatus(Request::TAKEN);
    req->setElevatorID(bestElevator);

    ui->console->insertPlainText("Request from floor # " + QString::number(req->getDestFloor()) + " going " + req->getDirection() + " has been taken by Elevator #" +QString::number(bestElevator)+"\n");

    ui->fpUp->setChecked(false);
    ui->fpDown->setChecked(false);

}

void MainWindow::epSubmitRequest(){
    int destFloor = ui->epDest->currentText().toInt();
    int elevatorID = ui->epElevator->currentIndex();
    Request* req = new Request(elevatorID, destFloor);

    for(Request* request: ecs->getElevators()[elevatorID]->getRequests()){
        if (request->getDestFloor() == destFloor && request->getSource()==Request::ELEVATOR && (request->getStatus()==Request::PENDING || request->getStatus()==Request::TAKEN)){
            ui->console->insertPlainText("There is a pending/currently taken request from Elevator # " + QString::number(request->getElevatorID()) + " to floor #"+QString::number(request->getDestFloor())+"\n");
            delete(req);
            return;
        }
    }

    ecs->getElevators()[elevatorID]->getRequests().append(req);
    ui->console->insertPlainText("Request from elevator #" + QString::number(elevatorID) + " to floor #" + QString::number(destFloor)+" has been initiaited\n");
}

void MainWindow::sfSubmitRequest(){
    ui->console->setTextColor(QColor(Qt::red));
    if(ui->sfFire->isChecked()){
        ui->console->insertPlainText("\nFIRE PROTOCOL ENGAGED\n");
        ecs->fireProtocol();
        ui->epWidget->setDisabled(true);
        ui->sfWidget->setDisabled(true);
        ui->fpWidget->setDisabled(true);
        ui->sfFire->setChecked(false);
    }
    else if(ui->sfOutage->isChecked()){
        ui->console->insertPlainText("\nOUTAGE PROTOCOL ENGAGED\n");
        ecs->outageProtocol();
        ui->epWidget->setDisabled(true);
        ui->sfWidget->setDisabled(true);
        ui->fpWidget->setDisabled(true);
        ui->sfOutage->setChecked(false);
    }
    else if(ui->sfObstacle->isChecked()){
        ui->console->insertPlainText("\nOBSTACLE PROTOCOL ENGAGED\n");
        ecs->obstacleProtocol(ecs->getElevators()[ui->sfElevator->currentIndex()]);
        ui->sfObstacle->setChecked(false);
    }
    else if(ui->sfEmergency->isChecked()){
        ui->console->insertPlainText("\nEMERGENCY PROTOCOL ENGAGED\n");
        ecs->helpProtocol(ecs->getElevators()[ui->sfElevator->currentIndex()]);
        ui->sfEmergency->setChecked(false);
    }
    else if(ui->sfOverload->isChecked()){
        ui->console->insertPlainText("\nOVERLOAD PROTOCOL ENGAGED\n");
        ecs->overloadProtocol(ecs->getElevators()[ui->sfElevator->currentIndex()]);
        ui->sfOverload->setChecked(false);
    }
    ui->console->setTextColor(QColor(Qt::black));
}

void MainWindow::onfpIndexChanged(){
    if(ui->fpFloor->currentIndex() == 0){
        ui->fpDown->setVisible(false);
    }
    else{
        ui->fpDown->setVisible(true);
    }

    if(ui->fpFloor->currentIndex() == this->numFloors-1){
        ui->fpUp->setVisible(false);
    }
    else{
        ui->fpUp->setVisible(true);
    }

}

void MainWindow::onsfButtonClicked() {
    ui->sfFire->setChecked(false);
    ui->sfOverload->setChecked(false);
    ui->sfEmergency->setChecked(false);
    ui->sfObstacle->setChecked(false);
    ui->sfOutage->setChecked(false);

    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        clickedButton->setCheckable(true);
    }
    if(clickedButton == ui->sfFire|| clickedButton == ui->sfOutage){
        ui->sfElevator->setDisabled(true);
    }
    else{
        ui->sfElevator->setEnabled(true);
    }
}

void MainWindow::onfpButtonClicked() {
    ui->fpDown->setChecked(false);
    ui->fpUp->setChecked(false);

    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        clickedButton->setCheckable(true);
    }
}

void MainWindow::onepElevatorChanged(){
    ui->epDest->clear();
    int elevID = ui->epElevator->currentIndex();
    if(ecs!=NULL){
        int reqElev = ecs->getElevators()[elevID]->getCurrFloor();
        for (int i = 0; i < this->numFloors; ++i) {
            if(i  != reqElev)
            ui->epDest->addItem(QString::number(i));
        }
    }
}

void MainWindow::proc(){
    onepElevatorChanged();
    ecs->next();

}

void MainWindow::printFromClass(QString text){
    ui->console->insertPlainText(text);
}

void MainWindow::onProcClicked(){
    if(ecs!=NULL && !ui->proc->isVisible())
    ui->proc->click();
}

void MainWindow::printState(){
    ecs->printState();
}
