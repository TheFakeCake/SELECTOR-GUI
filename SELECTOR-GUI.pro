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
    models/simulationmodel.cpp \
    models/deme.cpp \
    models/map.cpp \
    models/selection.cpp \
    models/MapStructureModifier.cpp \
    models/ModifierDeme.cpp \
    SelectorConfigFiles.cpp \
    gui/MapWidget.cpp \
    gui/IntervalWidget.cpp \
    gui/DemesParamWidget.cpp \
    gui/LogWidget.cpp \
    gui/CursorSelectTool.cpp \
    gui/BoxSelectTool.cpp

HEADERS  += mainwindow.h \
    models/simulationmodel.h \
    models/deme.h \
    models/map.h \
    models/selection.h \
    models/MapStructureModifier.h \
    models/ModifierDeme.h \
    SelectorConfigFiles.h \
    gui/MapWidget.h \
    gui/IntervalWidget.h \
    gui/DemesParamWidget.h \
    gui/LogWidget.h \
    gui/CursorSelectTool.h \
    gui/BoxSelectTool.h

FORMS    += mainwindow.ui \
    gui/IntervalWidget.ui \
    gui/DemesParamWidget.ui

RESOURCES += \
    icons.qrc
