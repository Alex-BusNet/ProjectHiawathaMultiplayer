#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T19:51:40
#
#-------------------------------------------------

QT       += core gui network concurrent
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
    Client/clienthandler.cpp \
    Common/diplomacy.cpp \
    Common/map.cpp \
    Common/techtree.cpp \
    Common/unitcontroller.cpp \
    Client/cityscreen.cpp \
    Client/notificationsystem.cpp \
    Client/renderer.cpp \
    Client/gamescene.cpp \
    Client/gameview.cpp \
    Server/ai_controller.cpp \
    Server/ai_operational.cpp \
    Server/ai_strategic.cpp \
    Server/ai_tactical.cpp

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
    Client/clienthandler.h \
    Common/diplomacy.h \
    Common/map.h \
    Common/techtree.h \
    Common/unitcontroller.h \
    Client/cityscreen.h \
    Client/notificationsystem.h \
    Client/renderer.h \
    Client/gamescene.h \
    Client/gameview.h \
    Server/ai_controller.h \
    Server/ai_operational.h \
    Server/ai_strategic.h \
    Server/ai_tactical.h

FORMS    += mainwindow.ui \
    Client/clientoptions.ui \
    Common/about.ui \
    Server/serveroptions.ui \
    Client/clientconnectdialog.ui \
    Common/diplomacy.ui \
    Common/techtree.ui \
    Client/cityscreen.ui \
    Server/servermanager.ui
