#-------------------------------------------------
#
# Project created by QtCreator 2016-05-02T12:13:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SELECTOR-GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    simulationmodel.cpp \
    deme.cpp \
    map.cpp \
    selection.cpp

HEADERS  += mainwindow.h \
    simulationmodel.h \
    deme.h \
    map.h \
    selection.h

FORMS    += mainwindow.ui
