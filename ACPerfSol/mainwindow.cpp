#include "mainwindow.h"
#include "ui_mainwindow.h"

//plot
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(400, 250, 542, 390);
    setupLineStyleDemo(ui->customPlot);
    setWindowTitle("QCustomPlot: "+demoName);
    statusBar()->clearMessage();
    ui->customPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
