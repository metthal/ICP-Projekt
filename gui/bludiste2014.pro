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
    ../src/gui/zoomgraphicsview.cpp \
    ../src/common/sentry.cpp \
    ../src/client/TcpClient.cpp \
    ../src/common/Packet.cpp \
    ../src/gui/dialoggamehistory.cpp \
    ../src/client/clientGame.cpp

HEADERS  += ../inc/gui/mainwindow.h \
    ../inc/gui/dialoggamemenu.h \
    ../inc/common/game.h \
    ../inc/common/levelmap.h \
    ../inc/common/position.h \
    ../inc/common/player.h \
    ../inc/common/msgexception.h \
    ../inc/gui/zoomgraphicsview.h \
    ../inc/common/sentry.h \
    ../inc/client/TcpClient.h \
    ../inc/common/Packet.h \
    ../inc/gui/dialoggamehistory.h \
    ../inc/client/clientGame.h

FORMS    += mainwindow.ui \
    dialoggamemenu.ui \
    dialoggamehistory.ui

LIBS += -lpthread -lboost_system -lboost_filesystem

DESTDIR = ../bin
OBJECTS_DIR = ../obj/gui
