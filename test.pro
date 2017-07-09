#-------------------------------------------------
#
# Project created by QtCreator 2017-07-03T14:03:21
#
#-------------------------------------------------

QT       += core gui

QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui
