#-------------------------------------------------
#
# Project created by QtCreator 2016-11-04T13:45:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4){
CONFIG += c++11
} else {
QMAKE_CXXFLAGS += -std=c++0x
}

TARGET = ACPerfSol
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Plot/qcustomplot.cpp \
    Plot/plotexamples.cpp \
    Plot/plotfunctions.cpp

HEADERS  += mainwindow.h \
    Accel/Accel.h \
    Accel/AccelClimbType.h \
    Accel/AccelDataPoint.h \
    Accel/AccelRPM.h \
    Accel/AccelTOC.h \
    Climb/Climb.h \
    Climb/ClimbDataPoint.h \
    Climb/ClimbISADev.h \
    Climb/ClimbMass.h \
    Climb/ClimbRPM.h \
    Climb/ClimbType.h \
    Cruise/Cruise.h \
    Cruise/CruiseDataPoint.h \
    Cruise/CruiseISADev.h \
    Cruise/CruiseMass.h \
    Cruise/CruiseType.h \
    Descent/Descent.h \
    Descent/DescentDataPoint.h \
    Descent/DescentISADev.h \
    Descent/DescentMass.h \
    Descent/DescentRPM.h \
    Descent/DescentType.h \
    Misc/Interpol.h \
    Misc/ISA.h \
    Misc/RouteFunctions.h \
    Misc/Wind.h \
    localinit.h \
    Plot/qcustomplot.h \
    Plot/plotfunctions.h

FORMS    += mainwindow.ui

DISTFILES += \
    Data/Climbdata.dat \
    Data/Acceldata.dat \
    Data/Cruisedata.dat \
    Data/Descentdata.dat

