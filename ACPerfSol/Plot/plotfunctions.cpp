#include "mainwindow.h"
#include "ui_mainwindow.h"

//plot
#include "Plot/qcustomplot.h"

#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>



void MainWindow::plotclimbdata(QCustomPlot *climbdataPlot)
{
    climbdataPlot->legend->setVisible(true);
    climbdataPlot->legend->setFont(QFont("Helvetica", 9));
    QPen pen;
    QStringList lineNames;
    lineNames << "climb data";


      climbdataPlot->addGraph();
      pen.setColor(QColor("red"));
      climbdataPlot->graph()->setPen(pen);
      climbdataPlot->graph()->setName(lineNames.at(QCPGraph::lsNone));
      //customPlot->graph()->setLineStyle((QCPGraph::LineStyle)i);
      //customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

      // generate data:
      QVector<double> x(15), y(15);
      for (int j=0; j<15; ++j)
      {
        x[j] = j+3;
        y[j] = j+3;
      }


      climbdataPlot->graph()->setData(x, y);
      climbdataPlot->graph()->rescaleAxes(true);


    /*
    // zoom out a bit:
    climbdataPlot->yAxis->scaleRange(1.1, climbdataPlot->yAxis->range().center());
    climbdataPlot->xAxis->scaleRange(1.1, climbdataPlot->xAxis->range().center());
    */
    /*
    // set blank axis lines:
    climbdataPlot->xAxis->setTicks(false);
    climbdataPlot->yAxis->setTicks(true);
    climbdataPlot->xAxis->setTickLabels(false);
    climbdataPlot->yAxis->setTickLabels(true);
    */
    // make top right axes clones of bottom left axes:
    climbdataPlot->axisRect()->setupFullAxesBox();
}

void MainWindow::plotcruisedata(QCustomPlot *cruisedataPlot)
{

}

void MainWindow::plotdescentdata(QCustomPlot *descentdataPlot)
{

}
