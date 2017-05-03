#-------------------------------------------------
#
# Project created by QtCreator 2017-04-01T23:32:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = StarMonitor
TEMPLATE = app

CONFIG += c++11

unix:INCLUDEPATH += /home/zzuummaa/cansat
SOURCES += /home/zzuummaa/cansat/*.cpp \
    telemetryloader.cpp \
    telemetryplot.cpp \
    realtimetelemetryloader.cpp \
    telemetry.cpp
HEADERS += /home/zzuummaa/cansat/*.h \
    telemetryloader.h \
    telemetryplot.h \
    realtimetelemetryloader.h \
    telemetry.h
SOURCES += /home/zzuummaa/cansat/shm/*.cpp
HEADERS += /home/zzuummaa/cansat/shm/*.h
SOURCES += /home/zzuummaa/cansat/ppm2bmp/*.cpp
HEADERS += /home/zzuummaa/cansat/ppm2bmp/*.h

unix:LIBS += -L/usr/lib -lredis3m

SOURCES += main.cpp\
        mainwindow.cpp \
        qcustomplot.cpp \

HEADERS  += mainwindow.h \
            qcustomplot.h \

FORMS    += mainwindow.ui
