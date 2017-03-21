#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T19:51:40
#
#-------------------------------------------------

QT       += core gui network
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = ProjectHiawathaMultiplayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Client/clientmanager.cpp \
    Server/servermanager.cpp \
    Server/serveroptions.cpp \
    Client/clientoptions.cpp \
    Common/about.cpp

HEADERS  += mainwindow.h \
    Client/clientmanager.h \
    Server/servermanager.h \
    Server/serveroptions.h \
    Client/clientoptions.h \
    Common/biome.h \
    Common/civcolors.h \
    Common/datatypes.h \
    Common/nation.h \
    Common/resources.h \
    Common/tileimprovement.h \
    Common/tiletype.h \
    Common/unittype.h \
    Common/yield.h \
    Common/about.h

FORMS    += mainwindow.ui \
    Client/clientoptions.ui \
    Common/about.ui \
    Server/serveroptions.ui
