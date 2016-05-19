QT += core
QT -= gui

TARGET = RazMSG
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ntripclient.cpp \
    sbas.cpp

HEADERS += \
    ntripclient.h \
    sbas.h

