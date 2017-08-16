#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QWidget>
#include <QTimer>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFuture>
#include <QtConcurrent>

#include "Server/serverhandler.h"
#include "Server/connectionmanager.h"
#include "Server/ai_controller.h"

#include <Common/civilization.h>
#include <Common/unit.h>
#include <Common/city.h>
#include <Common/tile.h>
#include <Common/unitcontroller.h>
#include <Common/diplomacy.h>

namespace Ui {
class ServerManager;
}

class ServerManager : public QWidget
{
    Q_OBJECT

public:
    explicit ServerManager(QWidget *parent = 0, int mapX = 40, int mapY = 24, QVector<int> civSelection = {16, 16}, QVector<bool> aiFlags = {false, false});

private:
    Ui::ServerManager *ui;

    /*
     * Server Setup variables
     */
    QTimer *updateTimer;
    QTimer *startGameTimer;
    int minHumanPlayers, connectedPlayers;
    UnitController *uc;
    Diplomacy *diplo;
    AI_Controller *ac;

    Map *map;

    QJsonDocument doc;
    QJsonObject obj;
    QJsonArray arr;

    QJsonArray TechData;
    QJsonArray BuildingData;
    QJsonArray UnitData;

    /*
     * Game Control Variables
     */
    QVector<Civilization*> civList;

    Unit* unitToMove, *targetUnit;
    City* targetCity;

    Tile *unitTile, *targetTile;

    int currentTurn, gameTurn;
    int foundCityIndex;
    int playersAliveCount;
    float year, yearPerTurn;
    bool turnStarted, turnEnded;
    ActionState state;
    QString statusMessage;

    QFuture<void> civInit, mapInit;

    /*
     * Game Control Functions
     */
    void InitCivs(QVector<int> civSel, QVector<bool> aiFlags);
    void GenerateRandomCiv(QJsonArray &civRefData, bool ai, QVector<int> &selNat);
    void LoadCivs();
//    void paintEvent(QPaintEvent *event);

    void TurnController();
    void StartTurn();
    void EndTurn();

    void Defeat();
    void Victory();

    void LoadJsonData();

    void UpdateTileData();

    void InitServerVariables();

    void ProcessCityConquer(City* tCity, Civilization* aCiv, Civilization* tCiv);
    void ProcessAttackUnit();
    void ProcessPeace(int makePeaceWithIndex);

    bool AcceptsPeace(Civilization* ai);

public slots:
    void WarDeclared();
    void WarAvoided();
    void WarByInvasion();
    void WarByDiplomacy();
    void MakePeace();
};

#endif // SERVERMANAGER_H
