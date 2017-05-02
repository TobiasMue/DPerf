#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Plot/qcustomplot.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupLineStyleDemo(QCustomPlot *customPlot);
    void plotclimbdata(QCustomPlot *climbdataPlot);
    void plotcruisedata(QCustomPlot *cruisedataPlot);
    void plotdescentdata(QCustomPlot *descentdataPlot);


private slots:
    void fcalculate_climb();
    void fcalculate_cruise();
    void fcalculate_descent();

private:
    Ui::MainWindow *ui;
    QString demoName;
    QTimer dataTimer;
    QCPItemTracer *itemDemoPhaseTracer;
    int currentDemoIndex;
};

#endif // MAINWINDOW_H
