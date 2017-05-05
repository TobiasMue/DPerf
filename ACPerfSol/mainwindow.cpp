#include "mainwindow.h"
#include "ui_mainwindow.h"

//plot
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

#include <iostream>
#include "Accel/Accel.h"
#include "Climb/Climb.h"
#include "Cruise/Cruise.h"
#include "Descent/Descent.h"
#include "Misc/RouteFunctions.h"
#include "Misc/Wind.h"
#include "Misc/ISA.h"
#include <localinit.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    TWindProfile ClimbWind;
        for(unsigned iFL = 0; iFL < 26; iFL += 2){
            ClimbWind.WindData(iFL * 10, (45 + 2*iFL)%360, 0 , 15 - iFL);
        }
    std::cout << "ClimbWind Loaded..." << std::endl;

    TWindProfile CruiseWind;
        for(unsigned iFL = 0; iFL < 25; iFL += 2){
            CruiseWind.WindData(iFL * 10, (45 + 2*iFL)%360, 0, 15 - iFL);
        }
    std::cout << "CruiseWind Loaded..." << std::endl;

    TWindProfile DescentWind;
        for(unsigned iFL = 0; iFL < 25; iFL += 2){
            DescentWind.WindData(iFL * 10, (45 + 2*iFL)%360, 0, 15 - iFL);
        }
    std::cout << "DescentWind Loaded..." << std::endl;

    /*##################*/
    //climb data
    // set start values (if different from index 0)
    ui->climb_comboBox_Source->setCurrentIndex(1); // 1 for new
    ui->climb_comboBox_FL->setCurrentIndex(21); //21 for FL 250
    // connect signal and slots
    connect(ui->climb_comboBox_Source, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_comboBox_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_comboBox_RPM, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_lineEdit_ISA, SIGNAL(textChanged(QString)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_comboBox_FL, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_lineEdit_Mass, SIGNAL(textChanged(QString)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_CheckBox_ICE, SIGNAL(stateChanged(int)), this, SLOT(fcalculate_climb()));
    // call function for the first time to fill results
    fcalculate_climb();

    /*##################*/
    //accel data
    // set start values (if different from index 0)

    // connect signal and slots

    // call function for the first time to fill results
    fcalculate_accel();

    /*##################*/
    //cruise data
    // set start values (if different from index 0)
    ui->cruise_comboBox_Source->setCurrentIndex(1); // 1 for new
    ui->cruise_comboBox_FL->setCurrentIndex(21); //21 for FL 250
    // connect signal and slots
    connect(ui->cruise_comboBox_Source, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_cruise()));
    connect(ui->cruise_comboBox_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_cruise()));
    connect(ui->cruise_lineEdit_ISA, SIGNAL(textChanged(QString)), this, SLOT(fcalculate_cruise()));
    connect(ui->cruise_comboBox_FL, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_cruise()));
    connect(ui->cruise_lineEdit_Mass, SIGNAL(textChanged(QString)), this, SLOT(fcalculate_cruise()));
    connect(ui->cruise_CheckBox_ICE, SIGNAL(stateChanged(int)), this, SLOT(fcalculate_cruise()));
    // call function for the first time to fill results
    fcalculate_cruise();

    /*##################*/
    //descent data
    // set start values (if different from index 0)
    ui->descent_comboBox_Source->setCurrentIndex(1); // 1 for new
    ui->descent_comboBox_FL->setCurrentIndex(21); //21 for FL 250
    // connect signal and slots
    connect(ui->descent_comboBox_Source, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_descent()));
    connect(ui->descent_comboBox_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_descent()));
    connect(ui->descent_lineEdit_ISA, SIGNAL(textChanged(QString)), this, SLOT(fcalculate_descent()));
    connect(ui->descent_comboBox_FL, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_descent()));
    connect(ui->descent_lineEdit_Mass, SIGNAL(textChanged(QString)), this, SLOT(fcalculate_descent()));
    // call function for the first time to fill results
    fcalculate_descent();





//#####################################################
    // test ui for ploting example
    //ui->setupUi(this);
    setGeometry(400, 250, 542, 390);
    setupLineStyleDemo(ui->customPlot);
    //setWindowTitle("QCustomPlot: "+demoName);
    statusBar()->clearMessage();
    ui->customPlot->replot();
//#####################################################


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::fcalculate_climb(){
    //get specific request values from lineEdit and comboBox
    double Clbsource, Clbtype, ClbRPM, Clbisadev, Clbfl, Clbmass, ClbICE;
    Clbtype = (ui->climb_comboBox_Type->currentText()).toDouble();
    ClbRPM = (ui->climb_comboBox_RPM->currentText()).toDouble();
    Clbisadev = (ui->climb_lineEdit_ISA->text()).toDouble();
    Clbfl = (ui->climb_comboBox_FL->currentText()).toDouble();
    Clbmass = (ui->climb_lineEdit_Mass->text()).toDouble();
    ClbICE = (ui->climb_CheckBox_ICE->checkState());
    //get specific request values from comboBox
    Clbsource = (ui->climb_comboBox_Source->currentIndex()); //FIXME index to combobox?
    // load specific climb data
    TClimbDataSet ClimbData(Clbsource);
    ClimbData.LoadData(CLIMBDATAPATH, ';', '\n');
    std::cout << "Climb Loaded..." << std::endl;
    // get specific datapoint
    double ClbTime = ClimbData.Time(ClbRPM, Clbtype, Clbisadev, Clbmass, Clbfl);
    double ClbAirDist = ClimbData.Dist(ClbRPM, Clbtype, Clbisadev, Clbmass, Clbfl);
    double ClbFuel = ClimbData.Fuel(ClbRPM, Clbtype, Clbisadev, Clbmass, Clbfl);
    if(ClbICE==2){ // 2 for ICE ON
        //FIXME
        ClbTime += ClbTime * ClimbData.AICorFuel(ClbRPM, Clbtype, Clbisadev) /100;
        ClbAirDist += ClbAirDist * ClimbData.AICorFuel(ClbRPM, Clbtype, Clbisadev) /100;
        ClbFuel += ClbFuel * ClimbData.AICorFuel(ClbRPM, Clbtype, Clbisadev) /100;
        }
    // write specific datapoint
    ui->climb_lineEdit_Result_Time->setText(QString("%1").arg(ClbTime,0,'f',2));
    ui->climb_lineEdit_Result_Dist->setText(QString("%1").arg(ClbAirDist,0,'f',2));
    ui->climb_lineEdit_Result_Fuel->setText(QString("%1").arg(ClbFuel,0,'f',2));
}

void MainWindow::fcalculate_accel(){
    //get specific request values from lineEdit and comboBox

    // load specific accel data
    TAccelDataSet AccelData(1);
    AccelData.LoadData(ACCELDATAPATH, ';', '\n');
    std::cout << "Accel Loaded..." << std::endl;
    // get specific datapoint



}
void MainWindow::fcalculate_cruise(){
    //get specific request values from lineEdit and comboBox
    double Crssource, Crstype, Crsisadev, Crsfl, Crsmass, CrsICE;
    Crstype = (ui->cruise_comboBox_Type->currentIndex())+1;
    Crsisadev = (ui->cruise_lineEdit_ISA->text()).toDouble();
    Crsfl = (ui->cruise_comboBox_FL->currentText()).toDouble();
    Crsmass = (ui->cruise_lineEdit_Mass->text()).toDouble();
    CrsICE = (ui->cruise_CheckBox_ICE->checkState());
    //get specific request values from comboBox
    Crssource = (ui->cruise_comboBox_Source->currentIndex());
    // load specific cruise data
    TCruiseDataSet CruiseData(Crssource);
    CruiseData.LoadData(CRUISEDATAPATH, ';', '\n');
    std::cout << "Cruise Loaded..." << std::endl;
    // get specific datapoint
    double CrsFF = CruiseData.FF(Crstype, Crsisadev, Crsmass, Crsfl);
    double CrsKIAS = CruiseData.KIAS(Crstype, Crsisadev, Crsmass, Crsfl);
    double CrsKTAS = CruiseData.KTAS(Crstype, Crsisadev, Crsmass, Crsfl);
    double CrsTRQ = CruiseData.TRQ(Crstype, Crsisadev, Crsmass, Crsfl);
    if(CrsICE==2){ // 2 for ICE ON
            CrsFF += CrsFF * CruiseData.AICorFF(Crstype, Crsisadev)/100;
            CrsTRQ += CrsTRQ * CruiseData.AICorTRQ(Crstype, Crsisadev)/100;
        }
    // write specific datapoint
    ui->cruise_lineEdit_Result_FuelFlow->setText(QString("%1").arg(CrsFF,0,'f',2));
    ui->cruise_lineEdit_Result_KIAS->setText(QString("%1").arg(CrsKIAS,0,'f',2));
    ui->cruise_lineEdit_Result_KTAS->setText(QString("%1").arg(CrsKTAS,0,'f',2));
    //ui->cruise_lineEdit_Result_TRQ->setText(QString("%1").arg(CrsTRQ,0,'f',2));
    //ploting climbdata

    //ui->setupUi(this);
    setGeometry(400, 250, 542, 390);
    plotclimbdata(ui->climbdataPlot);
    //setWindowTitle("QCustomPlot: ""climbdata");
    statusBar()->clearMessage();
    ui->climbdataPlot->replot();
}

void MainWindow::fcalculate_descent(){
    //get specific request values from lineEdit and comboBox
    double Descsource, DescRPM, Desctype, Descisadev, Descfl, Descmass;
    Desctype = (ui->descent_comboBox_Type->currentIndex())+1;
    DescRPM = (ui->descent_comboBox_RPM->currentText()).toDouble();
    Descisadev = (ui->descent_lineEdit_ISA->text()).toDouble();
    Descfl = (ui->descent_comboBox_FL->currentText()).toDouble();
    Descmass = (ui->descent_lineEdit_Mass->text()).toDouble();
    //get specific request values from comboBox
    Descsource = (ui->climb_comboBox_Source->currentIndex());
    // load specific descent data
    TDescentDataSet DescentData(Descsource);
    DescentData.LoadData(DESCENTDATAPATH, ';', '\n');
    std::cout << "Descent Loaded..." << std::endl;
    // get specific datapoint
    double DescTime = DescentData.Time(DescRPM, Desctype, Descisadev, Descmass, Descfl);
    double DescDist = DescentData.Dist(DescRPM, Desctype, Descisadev, Descmass, Descfl);
    double DescFF = DescentData.Fuel(DescRPM, Desctype, Descisadev, Descmass, Descfl);
    // write specific datapoint
    ui->descent_lineEdit_Result_Time->setText(QString("%1").arg(DescTime,0,'f',2));
    ui->descent_lineEdit_Result_Dist->setText(QString("%1").arg(DescDist,0,'f',2));
    ui->descent_lineEdit_Result_FuelFlow->setText(QString("%1").arg(DescFF,0,'f',2));
}
