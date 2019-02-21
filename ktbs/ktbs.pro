#-------------------------------------------------
#
# Project created by QtCreator 2015-02-23T13:46:43
#
#-------------------------------------------------

QT += network
QT += core
CONFIG += app_bundle
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ktbs
TEMPLATE = lib

DEFINES += KTBS_LIBRARY

SOURCES += ktbs.cpp

HEADERS += ktbs.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
