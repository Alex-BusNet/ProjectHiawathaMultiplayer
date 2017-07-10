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
    Common/about.cpp \
    Server/connectionmanager.cpp \
    Server/serverhandler.cpp \
    Server/messagequeue.cpp \
    Common/building.cpp \
    Common/city.cpp \
    Common/civilization.cpp \
    Common/unit.cpp \
    Common/yield.cpp \
    Common/tile.cpp \
    Common/technology.cpp \
    Common/queuedata.cpp \
    Client/clientconnectdialog.cpp \
    Client/clienthandler.cpp

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
    Common/about.h \
    Server/connectionmanager.h \
    Server/serverhandler.h \
    Server/messagequeue.h \
    Common/building.h \
    Common/city.h \
    Common/civilization.h \
    Common/unit.h \
    Common/tile.h \
    Common/technology.h \
    Common/queuedata.h \
    Client/clientconnectdialog.h \
    Client/clienthandler.h

FORMS    += mainwindow.ui \
    Client/clientoptions.ui \
    Common/about.ui \
    Server/serveroptions.ui \
    servermanager.ui \
    Client/clientconnectdialog.ui
