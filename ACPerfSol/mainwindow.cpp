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



    // try to load data for testing
    //std::cout << argv[0] << std::endl;

//    TClimbDataSet ClimbData(1);
//    ClimbData.LoadData(CLIMBDATAPATH, ';', '\n');
//    std::cout << "Climb Loaded..." << std::endl;
    TAccelDataSet AccelData(1);
    AccelData.LoadData(ACCELDATAPATH, ';', '\n');
    std::cout << "Accel Loaded..." << std::endl;
    TCruiseDataSet CruiseData(1);
    CruiseData.LoadData(CRUISEDATAPATH, ';', '\n');
    std::cout << "Cruise Loaded..." << std::endl;
    TDescentDataSet DescentData(1);
    DescentData.LoadData(DESCENTDATAPATH, ';', '\n');
    std::cout << "Descent Loaded..." << std::endl;

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


    //climb data
    /*##################*/

    // set start values (if different from index 0)
    ui->climb_comboBox_Source->setCurrentIndex(1); // 1 for new
    ui->climb_comboBox_FL->setCurrentIndex(21); //21 for FL 250

    connect(ui->climb_comboBox_Source, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_comboBox_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_comboBox_RPM, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_lineEdit_ISA, SIGNAL(textChanged(QString)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_comboBox_FL, SIGNAL(currentIndexChanged(int)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_lineEdit_Mass, SIGNAL(textChanged(QString)), this, SLOT(fcalculate_climb()));
    connect(ui->climb_CheckBox_ICE, SIGNAL(stateChanged(int)), this, SLOT(fcalculate_climb()));



    fcalculate_climb();







    // test ui for ploting example
    //ui->setupUi(this);
    setGeometry(400, 250, 542, 390);
    setupLineStyleDemo(ui->customPlot);
    //setWindowTitle("QCustomPlot: "+demoName);
    statusBar()->clearMessage();
    ui->customPlot->replot();


    // test2 ui for ploting climbdata
    //ui->setupUi(this);
    setGeometry(400, 250, 542, 390);
    plotclimbdata(ui->climbdataPlot);
    //setWindowTitle("QCustomPlot: ""climbdata");
    statusBar()->clearMessage();
    ui->climbdataPlot->replot();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::fcalculate_climb(){
    //get specific request values from lineEdit and comboBox
    double Clbsource, Clbtype, ClbRPM, Clbisadev, Clbfl, Clbmass, ClbICE;

Clbsource = (ui->climb_comboBox_Source->currentIndex()); //FIXME index to combobox?
Clbtype = (ui->climb_comboBox_Type->currentText()).toDouble();
ClbRPM = (ui->climb_comboBox_RPM->currentText()).toDouble();
Clbisadev = (ui->climb_lineEdit_ISA->text()).toDouble();
Clbfl = (ui->climb_comboBox_FL->currentText()).toDouble();
Clbmass = (ui->climb_lineEdit_Mass->text()).toDouble();
ClbICE = (ui->climb_CheckBox_ICE->checkState());


// load specific climb data
TClimbDataSet ClimbData(Clbsource);
ClimbData.LoadData(CLIMBDATAPATH, ';', '\n');
std::cout << "Climb Loaded..." << std::endl;

// get specific datapoints
double ClbTime = ClimbData.Time(ClbRPM, Clbtype, Clbisadev, Clbmass, Clbfl);
double ClbAirDist = ClimbData.Dist(ClbRPM, Clbtype, Clbisadev, Clbmass, Clbfl);
double ClbFuel = ClimbData.Fuel(ClbRPM, Clbtype, Clbisadev, Clbmass, Clbfl);
if(ClbICE==2){ // 2 for ICE ON
    //FIXME
    //ClbTime += ClbTime * AICorFuel/100;
    //ClbAirDist += ClbAirDist * AICorAirDist/100;
    //ClbFuel += ClbFuel * AICorFuel/100;

}



// write specific datapoints
ui->climb_lineEdit_Result_Time->setText(QString("%1").arg(ClbTime,0,'f',2));
ui->climb_lineEdit_Result_Dist->setText(QString("%1").arg(ClbAirDist,0,'f',2));
ui->climb_lineEdit_Result_Fuel->setText(QString("%1").arg(ClbFuel,0,'f',2));

}
