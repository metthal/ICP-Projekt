#-------------------------------------------------
#
# Project created by QtCreator 2014-03-06T00:36:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

TARGET = bludiste2014
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialoggamemenu.cpp

HEADERS  += mainwindow.h \
    dialoggamemenu.h

FORMS    += mainwindow.ui \
    dialoggamemenu.ui
