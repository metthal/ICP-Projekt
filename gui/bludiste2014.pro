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

INCLUDEPATH += ../inc/

SOURCES += ../src/gui/main.cpp\
        ../src/gui/mainwindow.cpp \
    ../src/gui/dialoggamemenu.cpp \
    ../src/common/game.cpp \
    ../src/common/levelmap.cpp \
    ../src/common/position.cpp \
    ../src/common/player.cpp \
    ../src/common/msgexception.cpp \
    ../src/gui/zoomgraphicsview.cpp

HEADERS  += ../inc/gui/mainwindow.h \
    ../inc/gui/dialoggamemenu.h \
    ../inc/common/game.h \
    ../inc/common/levelmap.h \
    ../inc/common/position.h \
    ../inc/common/player.h \
    ../inc/common/msgexception.h \
    ../inc/gui/zoomgraphicsview.h

FORMS    += mainwindow.ui \
    dialoggamemenu.ui

DESTDIR = ../bin
OBJECTS_DIR = ../obj/gui