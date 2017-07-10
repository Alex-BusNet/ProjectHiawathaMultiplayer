#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QTimer>
#include <QWidget>
#include "Server/serverhandler.h"
#include <Common/civilization.h>
#include <Common/unit.h>
#include <Common/city.h>
#include <Common/tile.h>

namespace Ui {
class ServerManager;
}

class ServerManager : public QWidget
{
    Q_OBJECT
public:
    explicit ServerManager(QWidget *parent = 0, int mapX = 40, int mapY = 24, int numAI = 0);

private:
    Ui::ServerManager *ui;

    /*
     * Server Setup variables
     */
    QTimer *updateTimer;
    QTimer *startGameTimer;
    int minHumanPlayers, connectedPlayers;

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

    /*
     * Game Control Functions
     */
    void InitCivs(Nation player, int numAI);
    void LoadCivs();
//    void paintEvent(QPaintEvent *event);

    void TurnController();
    void StartTurn();
    void EndTurn();

    void UpdateTileData();

    void InitVariables(bool fullscreen);
};

#endif // SERVERMANAGER_H
