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

    TClimbDataSet ClimbData(1);
    ClimbData.LoadData(CLIMBDATAPATH, ';', '\n');
    std::cout << "Climb Loaded..." << std::endl;
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
