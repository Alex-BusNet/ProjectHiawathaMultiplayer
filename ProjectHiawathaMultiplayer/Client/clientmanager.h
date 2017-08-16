#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "cityscreen.h"
#include "gameview.h"
#include "notificationsystem.h"
#include "renderer.h"

#include <QDesktopWidget>
#include <QFuture>
#include <QJsonArray>
#include <QListWidget>
#include <QMessageBox>
#include <QQueue>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include <Common/datatypes.h>
#include <Common/civilization.h>
#include <Common/nation.h>
#include <Common/techtree.h>
#include <Common/unitcontroller.h>
#include <Common/about.h>
#include <Common/diplomacy.h>

class ClientManager : public QWidget
{
    Q_OBJECT
public:
    explicit ClientManager(QWidget *parent = 0, bool fullscreen = false, Nation player = India,
                           QString IP = "127.0.0.1", QString username = "Sparta");
    ~ClientManager();

    void LoadCivData(QJsonArray arr);
    void LoadMapData(QJsonObject obj);
    void LoadDiploData(QJsonObject obj);

    void UpdateGameData(MessageTypes type, QJsonObject obj);

    void paintEvent(QPaintEvent *event);


private:

    //====================
    // Game Functions
    void InitClientVariables(bool fullscreen);
    void InitButtons();
    void InitLayouts();
    void InitYieldDisplay();
    void InitRenderData();

    void Render();
    void LoadJsonData();
    //====================

    //====================
    // Game Variables
    QVector<Civilization*> civList;
    QVector<QGraphicsTextItem*> stringData;

    int thisPlayer;
    Nation playerNation;

    int zoomScale;
    int currentTurn, gameTurn;
    int foundCityIndex;
    int playersAliveCount;

    float year, yearPerTurn;

    bool mapReady, civsReady;
    bool redrawTile;
    bool cityScreenVisible, techTreeVisible, diploVisible;
    bool toggleOn, relocateUnit, turnEnded, turnStarted;
    bool countTime, citySelected, updateFoW, focusChanged, fortify;

    Map *map;
    About *about;
    QMessageBox *warBox;
    GameView *gameView;
    Renderer *renderer;
    NotificationSystem *ns;
    Diplomacy *diplo;
    QListWidget *clv;
    UnitController *uc;
    CityScreen *cityScreen;
    TechTree *techTree;

    Unit *unitToMove, *targetUnit;
    Tile *unitTile, *targetTile;
    City *targetCity;
    ActionState state;

    TileData processedData;

    QJsonArray BuildingData;
    QJsonArray UnitData;
    QJsonArray TechData;

    QVBoxLayout *vLayout;
    QVBoxLayout *playerControlButtons;
    QVBoxLayout *unitControlButtons;

    QHBoxLayout *hLayout;
    QHBoxLayout *gameLayout;
    QHBoxLayout *yieldInfo;

    QQueue<SelectData> *selectedTileQueue;
    QQueue<SelectData> *tileModifiedQueue;
    QQueue<ViewData> *viewUpdateTiles;

    QLabel *endGameProgress;
    QLabel *techLabel, *techText;
    QLabel *goldLabel, *goldText;
    QLabel *prodLabel, *prodText;
    QLabel *sciLabel, *sciText;
    QLabel *foodLabel, *foodText;
    QLabel *culLabel, *culText;

    QString *endGameText;
    QString currentProductionName;
    QString IP, username;
    QString statusMessage;

    QPixmap *goldPix, *prodPix, *sciPix, *foodPix, *culPix;
    QRect *playerInfoRect, *gameStatusRect;

    QPushButton *exitGame;
    QPushButton *endTurn;
    QPushButton *moveUnit;
    QPushButton *showTechTreeButton;
    QPushButton *showDiplomacy;
    QPushButton *buildFarm;
    QPushButton *buildMine;
    QPushButton *buildTradePost;
    QPushButton *buildPlantation;
    QPushButton *buildRoad;
    QPushButton *buildQuarry;
    QPushButton *buildFishBoat;
    QPushButton *buildCamp;
    QPushButton *buildOilWell;
    QPushButton *buildPasture;
    QPushButton *foundCity;
    QPushButton *attackUnit;
    QPushButton *attackCity;
    QPushButton *rangeAttack;
    QPushButton *fortifyUnit;
    QPushButton *help;

    QPushButton *goldFocus;
    QPushButton *productionFocus;
    QPushButton *scienceFocus;
    QPushButton *foodFocus;
    QPushButton *cultureFocus;

    QTimer *updateTimer;
    QDesktopWidget widget;
    //===================

private slots:
    void closeGame();

    void zoomIn();
    void zoomOut();
    void showCity(City* city);
    void UpdateTiles();
    void moveUnitTo();
    void nextTurn();
    void showTechTree();
    void toggleDiplomacy();
    void foundNewCity();
    void buildNewRoad();
    void buildNewFarm();
    void buildNewPlantation();
    void buildNewTradePost();
    void buildNewMine();
    void buildNewCamp();
    void buildNewPasture();
    void buildNewOilWell();
    void buildNewFishBoat();
    void buildNewQuarry();
    void attackMelee();
    void SetGoldFocus();
    void SetProdFocus();
    void SetScienceFocus();
    void SetFoodFocus();
    void SetCultureFocus();
    void AttackCity();
    void RangeAttack();
    void Fortify();
    void WarDeclared();
    void WarAvoided();
    void WarByInvasion();
    void WarByDiplomacy();
    void MakePeace();
    void OpenHelp();

    void parseItem();
//    void toggleFog();

};

#endif // CLIENTMANAGER_H
