#include "clienthandler.h"
#include "clientmanager.h"

#include <QMessageBox>
#include <QTime>

QPen gmPen(Qt::black);
QBrush gmBrush(Qt::black);
QString warStyle = "QMessageBox { background-color: #145e88 } QPushButton {  background-color: #4899C8; border: 2px solid #f6b300; border-radius: 3px; font: 10px; min-width: 70px; } QPushButton#Cancel { background-color: #f53252 } QPushButton:pressed { background-color: #77adcb; }";

ClientManager::ClientManager(QWidget *parent, bool fullscreen, Nation player, QString IP, QString username) :
    QWidget(parent)
{
    qDebug() << "[ClientManager]" << "Client Manager Starting.";

    this->setWindowTitle("Project Hiawatha Client");
    ClientHandler::instance()->SetClientManagerObject(this);

    playerNation = player;
    this->IP = IP;
    this->username = username;

//    QMessageBox *mbox = new QMessageBox();
//    mbox->setWindowTitle("Waiting...");
//    mbox->setText("Waiting on server.");
//    mbox->setStyleSheet(warStyle);

    InitClientVariables(fullscreen);
//    mbox->show();

    qDebug() << "[ClientHandler]" << "Waiting on server...";
//    while(!mapReady || !civsReady) {;}

//    if(mbox->isVisible())
//        mbox->hide();

//    qDebug() << "[ClientManager]" << "All Data received. Initializing Render data";
//    InitRenderData();
//    qDebug() << "[ClientManager]" << "Done";

}

ClientManager::~ClientManager()
{

}

void ClientManager::LoadCivData(QJsonArray arr)
{
    qDebug() << "[ClientManager]" << "LoadCivData";
    for(int i = 0; i < arr.size(); i++)
    {
        Civilization* civ = new Civilization(arr[i].toObject());
        civList.push_back(civ);

        if(civ->getCiv() == playerNation)
            thisPlayer = i;
    }

    techLabel->setText(QString(" %1 ").arg(civList[thisPlayer]->getCurrentTech()->getName()));
    civsReady = true;

    qDebug() << "[ClientManager]" << "Local player: " << thisPlayer;
}

void ClientManager::LoadMapData(QJsonObject obj)
{
    qDebug() <<"[ClienManager]" << "LoadMapData";
    map->ReadMapSaveData(obj);
}

void ClientManager::LoadDiploData(QJsonObject obj)
{
    qDebug() << "[ClientManager]" << "LoadDiploData";
    diplo->ReadDiploSaveData(obj);
}

void ClientManager::UpdateGameData(MessageTypes type, QJsonObject obj)
{
    if(type == MAP_UPDATE_DATA)
    {
        qDebug() << "[ClientHandler]" << "Map Update Data recieved.";
        map->ReadMapUpdateData(obj["tileupdates"].toArray());
        mapReady = true;
    }
    else if(type == DIPLO_UPDATE_DATA)
    {
        qDebug() << "[ClientHandler]" << "Diplo Update Data recieved.";
        diplo->ReadDiploUpdateData(obj);
    }
    else if(type == CIV_UPDATE_DATA)
    {
        qDebug() << "[ClientHandler]" << "Civ Update Data recieved.";
        // obj contains array of civs that have been updated
    }
    else if(type == UNIT_UPDATE_DATA)
    {
        qDebug() << "[ClientHandler]" << "Unit Update Data recieved.";
    }
    else if(type == BUILDING_UPDATE_DATA)
    {
        qDebug() << "[ClientHandler]" << "Building Update Data recieved.";
    }

    if(mapReady && civsReady)
    {
        InitRenderData();
        updateTimer->start();
    }
}

void ClientManager::paintEvent(QPaintEvent *event)
{
    if(mapReady && civsReady && renderReady)
    {
        QPainter paint(this);
        paint.fillRect(*playerInfoRect, QBrush(Qt::black));
        paint.fillRect(*gameStatusRect, QBrush(Qt::black));
        paint.setPen(Qt::white);
        paint.drawText(*playerInfoRect, (Qt::AlignRight | Qt::AlignVCenter), QString("Turn %1 | %2 %3  ").arg(gameTurn).arg(abs(year)).arg((year < 0) ? "BC" : "AD"));

        paint.drawText(*gameStatusRect, (Qt::AlignHCenter | Qt::AlignVCenter), statusMessage);
    }
}

void ClientManager::Render()
{

}

void ClientManager::LoadJsonData()
{
    QFile buildExport("Data/buildings.json");

    if(!buildExport.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open buildExport save file");
    }
    else
    {
        QByteArray data = buildExport.readAll();
        QJsonDocument build = QJsonDocument::fromJson(data);
        BuildingData = build.array();
    }

    QFile unitExport("Data/units.json");

    if(!unitExport.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open unitExport save file");
    }
    else
    {
        QByteArray data = unitExport.readAll();
        QJsonDocument unit = QJsonDocument::fromJson(data);
        UnitData = unit.array();

    }

    QFile techExport("Data/techList.json");

    if(!techExport.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open techExport save file");
    }
    else
    {
        QByteArray data = techExport.readAll();
        QJsonDocument tech = QJsonDocument::fromJson(data);
        TechData = tech.array();
    }
}

void ClientManager::closeGame()
{

}

void ClientManager::zoomIn()
{

}

void ClientManager::zoomOut()
{

}

void ClientManager::showCity(City *city)
{

}

void ClientManager::InitClientVariables(bool fullscreen)
{
    qDebug() << "[ClientManager]" << "InitClientVariables";
    map = new Map();
    gameView = new GameView(this);
    clv = new QListWidget(this);
    uc = new UnitController();
    ns = new NotificationSystem(this);
    diplo = new Diplomacy(this);
    diplo->hide();
    techTree = new TechTree(this);

    warBox = new QMessageBox();
    warBox->addButton(QMessageBox::Cancel);
    warBox->addButton(QMessageBox::Ok);
    warBox->button(QMessageBox::Ok)->setText("Go to War");
    warBox->button(QMessageBox::Cancel)->setText("Let me reconsider...");
    warBox->setStyleSheet(warStyle);

    vLayout = new QVBoxLayout();
    hLayout = new QHBoxLayout();
    gameLayout = new QHBoxLayout();
    playerControlButtons = new QVBoxLayout();
    unitControlButtons = new QVBoxLayout();
    yieldInfo = new QHBoxLayout();

    selectedTileQueue = new QQueue<SelectData>();
    tileModifiedQueue = new QQueue<SelectData>();
    viewUpdateTiles = new QQueue<ViewData>();

    cityScreen = new CityScreen();
    cityScreen->hide();

    techLabel = new QLabel(" NO RESEARCH ");
    techText = new QLabel(" 00/000 ");
    endGameProgress = new QLabel("Capitals Controlled:");
    endGameText = new QString(" 0/0 ");

    unitToMove = NULL;
    targetUnit = NULL;
    targetCity = NULL;
    unitTile = NULL;
    targetTile = NULL;
    state = IDLE;

    cityScreenVisible = false;
    techTreeVisible = false;
    diploVisible = false;
    toggleOn = false;
    relocateUnit = false;
    turnEnded = false;
    turnStarted = true;
    countTime = false;
    citySelected = false;
    updateFoW = false;
    focusChanged = false;
    fortify = false;
    mapReady = false;
    civsReady = false;
    renderReady = false;

    currentProductionName = "No Production Selected";

    if(!fullscreen)
    {
        this->setFixedSize(1400, 700);
        this->setWindowTitle("Project Hiawatha Client");
        gameView->setFixedWidth(1195);
        this->setWindowFlags(Qt::WindowMinimizeButtonHint);
    }
    else
    {
        gameView->setMaximumWidth(widget.screenGeometry(0).width() - 200);
        this->setWindowState(Qt::WindowFullScreen);
    }

    gameView->ConfigureGraphicsView();

    updateTimer = new QTimer();
    updateTimer->setInterval(17);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(UpdateTiles()));
//    updateTimer->start();

    qsrand(QTime::currentTime().msec());
    this->InitButtons();
    this->InitLayouts();
    this->setLayout(vLayout);

    this->show();
    ns->hide();
}

void ClientManager::InitButtons()
{
    qDebug() << "[ClientManager]" << "InitButtons";

    QString GameStyle = "QWidget { background-color: #145e88; } QFrame { background-color: black; }";
    GameStyle += "QPushButton { background-color: #4899C8; border: 1px solid black; border-radius: 6px; font: 10px; max-width: 100px; }";
    GameStyle += "QPushButton:pressed { background-color: #77adcb; }";
    GameStyle += "QScrollBar:vertical { border: 2px sold black; background: #77adcb; width: 15px; margin: 12px 0 12px 0;} QScrollBar::handle:vertical { background: #4899C8; min-height: 10px; }";
    GameStyle += "QScrollBar::add-line:vertical { border: 1px solid black; background: #dedede; height: 10px; subcontrol-position: bottom; subcontrol-origin: margin; }  QScrollBar::sub-line:vertical { border: 1px solid black; height: 10px; background: #dedede; subcontrol-position: top; subcontrol-origin: margin; }";
    GameStyle += "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { border: 1px solid black; width: 3px; height: 3px; background: purple; } QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }";
    GameStyle += "QTabBar::tab { background: #dedede; border: 1px solid black; min-width: 10ex; padding: 3px;} QTabBar::tab:selected { background: #4899C8; } QTabBar::tab:hover { background: #77adcb; }";
    GameStyle += "QGraphicsView { background-color: transparent; border-color: transparent;}";
    GameStyle += "NotificationSystem { background-color: transparent; border-color: transparent; }";
    GameStyle += "QListWidget { background-color: grey; color: white; border: 3px inset black; }";
    GameStyle += "QProgressBar { border: 2px solid grey; border-radius: 5px; } QProgressBar::chunk { background-color: #CD96CD; }";
    GameStyle += "QLabel { background-color: transparent; color: white; }";

    this->setStyleSheet(GameStyle);

    exitGame = new QPushButton("Exit To Menu");
    connect(exitGame, SIGNAL(clicked(bool)), this, SLOT(closeGame()));
    exitGame->setShortcut(QKeySequence(Qt::Key_Escape));

    showTechTreeButton = new QPushButton("Technology Tree");
    connect(showTechTreeButton, SIGNAL(clicked(bool)), this, SLOT(showTechTree()));
    showTechTreeButton->setShortcut(QKeySequence(Qt::Key_T));

    showDiplomacy = new QPushButton("Diplomacy");
    connect(showDiplomacy, SIGNAL(clicked(bool)), this, SLOT(toggleDiplomacy()));
    showDiplomacy->setShortcut(QKeySequence(Qt::Key_V));

    moveUnit = new QPushButton("Move Unit");
    connect(moveUnit, SIGNAL(clicked(bool)), this, SLOT(moveUnitTo()));
    moveUnit->setEnabled(false);
    moveUnit->setShortcut(QKeySequence(Qt::RightButton));

    endTurn = new QPushButton("End Turn");
    connect(endTurn, SIGNAL(clicked(bool)), this, SLOT(nextTurn()));
    endTurn->setShortcut(QKeySequence(Qt::Key_Return));

    buildFarm = new QPushButton("Build Farm");
    connect(buildFarm, SIGNAL(clicked(bool)), this, SLOT(buildNewFarm()));
    buildFarm->setEnabled(false);
    buildFarm->setShortcut(QKeySequence(Qt::Key_F));

    buildMine = new QPushButton("Build Mine");
    connect(buildMine, SIGNAL(clicked(bool)), this, SLOT(buildNewMine()));
    buildMine->setEnabled(false);
    buildMine->setShortcut(QKeySequence(Qt::Key_S));

    buildPlantation = new QPushButton("Build Plantation");
    connect(buildPlantation, SIGNAL(clicked(bool)), this, SLOT(buildNewPlantation()));
    buildPlantation->setEnabled(false);
    buildPlantation->setShortcut(QKeySequence(Qt::Key_D));

    buildTradePost = new QPushButton ("Build Trading Post");
    connect(buildTradePost, SIGNAL(clicked(bool)), this, SLOT(buildNewTradePost()));
    buildTradePost->setEnabled(false);
    buildTradePost->setShortcut(QKeySequence(Qt::Key_G));

    buildRoad = new QPushButton("Build Road");
    connect(buildRoad, SIGNAL(clicked(bool)), this, SLOT(buildNewRoad()));
    buildRoad->setEnabled(false);
    buildRoad->setShortcut(QKeySequence(Qt::Key_H));

    buildCamp = new QPushButton("Build Camp");
    connect(buildCamp, SIGNAL(clicked(bool)), this, SLOT(buildNewCamp()));
    buildCamp->setEnabled(false);
    // Shortcut

    buildPasture = new QPushButton("Build Pasture");
    connect(buildPasture, SIGNAL(clicked(bool)), this, SLOT(buildNewPasture()));
    buildPasture->setEnabled(false);
    // Shortcut

    buildFishBoat = new QPushButton("Build Fishing Boat");
    connect(buildFishBoat, SIGNAL(clicked(bool)), this, SLOT(buildNewFishBoat()));
    buildFishBoat->setEnabled(false);
    // Shortcut

    buildQuarry = new QPushButton("Build Quarry");
    connect(buildQuarry, SIGNAL(clicked(bool)), this, SLOT(buildNewQuarry()));
    buildQuarry->setEnabled(false);
    // Shortcut

    buildOilWell = new QPushButton("Build Oil Well");
    connect(buildOilWell, SIGNAL(clicked(bool)), this, SLOT(buildNewOilWell()));
    buildOilWell->setEnabled(false);
    // Shortcut

    foundCity = new QPushButton("Found City");
    connect(foundCity, SIGNAL(clicked(bool)), this, SLOT(foundNewCity()));
    foundCity->setEnabled(false);
    foundCity->setShortcut(QKeySequence(Qt::Key_Q));

    attackUnit = new QPushButton("Attack");
    connect(attackUnit, SIGNAL(clicked(bool)), this, SLOT(attackMelee()));
    attackUnit->setEnabled(false);
    attackUnit->setShortcut(QKeySequence(Qt::Key_W));

    goldFocus = new QPushButton("Gold Focus");
    connect(goldFocus, SIGNAL(clicked(bool)), this, SLOT(SetGoldFocus()));
    goldFocus->setEnabled(false);
    goldFocus->setMaximumWidth(100);

    productionFocus = new QPushButton("Production Focus");
    connect(productionFocus, SIGNAL(clicked(bool)), this, SLOT(SetProdFocus()));
    productionFocus->setEnabled(false);
    productionFocus->setMaximumWidth(100);

    scienceFocus = new QPushButton("Science Focus");
    connect(scienceFocus, SIGNAL(clicked(bool)), this, SLOT(SetScienceFocus()));
    scienceFocus->setEnabled(false);
    scienceFocus->setMaximumWidth(100);

    foodFocus = new QPushButton("Food Focus");
    connect(foodFocus, SIGNAL(clicked(bool)), this, SLOT(SetFoodFocus()));
    foodFocus->setEnabled(false);
    foodFocus->setMaximumWidth(100);

    cultureFocus = new QPushButton("Culture Focus");
    connect(cultureFocus, SIGNAL(clicked(bool)), this, SLOT(SetCultureFocus()));
    cultureFocus->setEnabled(false);
    cultureFocus->setMaximumWidth(100);

    connect(clv, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(parseItem()));
//    connect(ns, SIGNAL(itemClicked(QListWidgetItem*)), ns, SLOT(removeNotification(QListWidgetItem*)));
    connect(diplo->makePeace, SIGNAL(clicked(bool)), this, SLOT(MakePeace()));
    connect(diplo->declareWar, SIGNAL(clicked(bool)), this, SLOT(WarByDiplomacy()));

    clv->setMaximumWidth(100);

    attackCity = new QPushButton("Attack City");
    connect(attackCity, SIGNAL(clicked(bool)), this, SLOT(AttackCity()));
    attackCity->setEnabled(false);
    attackCity->setShortcut(QKeySequence(Qt::Key_E));

    rangeAttack = new QPushButton("Range Attack");
    connect(rangeAttack, SIGNAL(clicked(bool)), this, SLOT(RangeAttack()));
    rangeAttack->setEnabled(false);
    rangeAttack->setShortcut(QKeySequence(Qt::Key_R));

    fortifyUnit = new QPushButton("Fortify");
    connect(fortifyUnit, SIGNAL(clicked(bool)), this, SLOT(Fortify()));
    fortifyUnit->setEnabled(false);
    fortifyUnit->setShortcut(QKeySequence(Qt::Key_A));

    help = new QPushButton("Help");
    connect(help, SIGNAL(clicked(bool)), this, SLOT(OpenHelp()));
    help->setShortcut(QKeySequence(Qt::Key_Z));

//    toggleFoW = new QPushButton("Toggle FoW");
//    connect(toggleFoW, SIGNAL(clicked(bool)), this, SLOT(toggleFog()));
}

void ClientManager::InitLayouts()
{
    qDebug() << "[ClientManager]" << "InitLayouts";
    vLayout->setMargin(2);

    unitControlButtons->addWidget(showTechTreeButton);
    unitControlButtons->addWidget(showDiplomacy);
    unitControlButtons->addSpacerItem(new QSpacerItem(100, 500, QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    unitControlButtons->addWidget(attackCity);
    unitControlButtons->addWidget(rangeAttack);
    unitControlButtons->addWidget(attackUnit);
    unitControlButtons->addWidget(fortifyUnit);
    unitControlButtons->addWidget(foundCity);
    unitControlButtons->addWidget(buildFarm);
    unitControlButtons->addWidget(buildMine);
    unitControlButtons->addWidget(buildPlantation);
    unitControlButtons->addWidget(buildTradePost);
    unitControlButtons->addWidget(buildCamp);
    unitControlButtons->addWidget(buildOilWell);
    unitControlButtons->addWidget(buildPasture);
    unitControlButtons->addWidget(buildQuarry);
    unitControlButtons->addWidget(buildFishBoat);
    unitControlButtons->addWidget(buildRoad);
    unitControlButtons->addWidget(moveUnit);
    unitControlButtons->setGeometry(QRect(0, 20, 100, this->height() - 20));
    unitControlButtons->setSizeConstraint(QVBoxLayout::SetFixedSize);

    gameLayout->addLayout(unitControlButtons);
    gameLayout->addWidget(gameView);
    gameLayout->addWidget(techTree);

//    if(this->isFullScreen())
//        ns->setGeometry(this->width() - 150, 20, 50, this->height() - 40);
//    else
    gameLayout->addWidget(ns);

    gameLayout->setGeometry(QRect(100, 20, this->width(), this->height()));
    diplo->setGeometry(gameView->pos().x() + 5, gameView->pos().y() + 2, this->width(), this->height());


    QFrame *frame = new QFrame(this);
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Sunken);
    frame->setLineWidth(1);

    playerControlButtons->addWidget(exitGame);
    playerControlButtons->addWidget(help);
    playerControlButtons->addWidget(clv);
    playerControlButtons->addWidget(endGameProgress);
    playerControlButtons->addWidget(frame);
    playerControlButtons->addWidget(techLabel);
    playerControlButtons->addWidget(techText);
    playerControlButtons->addWidget(goldFocus);
    playerControlButtons->addWidget(productionFocus);
    playerControlButtons->addWidget(scienceFocus);
    playerControlButtons->addWidget(foodFocus);
    playerControlButtons->addWidget(cultureFocus);
    playerControlButtons->addWidget(endTurn);
    playerControlButtons->setGeometry(QRect(this->width() - 100, 20, 100, this->height() - 20));

    gameLayout->addLayout(playerControlButtons);

    vLayout->addLayout(gameLayout);
    vLayout->addSpacing(20);
}

void ClientManager::InitYieldDisplay()
{
    goldText = new QLabel(QString("%1 (+%2)").arg(civList.at(thisPlayer)->GetTotalGold()).arg(civList.at(thisPlayer)->getCivYield()->GetGoldYield()));
    prodText = new QLabel(QString("%1").arg(civList.at(thisPlayer)->getCivYield()->GetProductionYield()));
    foodText = new QLabel(QString("%1").arg(civList.at(thisPlayer)->getCivYield()->GetFoodYield()));
    sciText = new QLabel(QString("%1 (+%2)").arg(civList.at(thisPlayer)->GetTotalScience()).arg(civList.at(thisPlayer)->getCivYield()->GetScienceYield()));
    culText = new QLabel(QString("%1").arg(civList.at(thisPlayer)->getCivYield()->GetCultureYield()));

    goldText->setStyleSheet("QLabel { color: white; }");
    prodText->setStyleSheet("QLabel { color: white; }");
    foodText->setStyleSheet("QLabel { color: white; }");
    sciText->setStyleSheet("QLabel { color: white; }");
    culText->setStyleSheet("QLabel { color: white; }");

    goldText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    prodText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    foodText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    sciText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    culText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

#ifdef __APPLE__
QDir bin(QCoreApplication::applicationDirPath());
bin.cdUp();
bin.cdUp();
bin.cdUp();
QDir::setCurrent(bin.absolutePath());
#endif

    goldPix = new QPixmap("Assets/Icons/gold.png");
    prodPix = new QPixmap("Assets/Icons/production.png");
    foodPix = new QPixmap("Assets/Icons/food.png");
    sciPix = new QPixmap("Assets/Icons/research.png");
    culPix = new QPixmap("Assets/Icons/culture.png");

    goldLabel = new QLabel();
    prodLabel = new QLabel();
    foodLabel = new QLabel();
    sciLabel = new QLabel();
    culLabel = new QLabel();

    goldLabel->setPixmap(*goldPix);
    prodLabel->setPixmap(*prodPix);
    foodLabel->setPixmap(*foodPix);
    sciLabel->setPixmap(*sciPix);
    culLabel->setPixmap(*culPix);

    yieldInfo->addWidget(goldLabel);
    yieldInfo->addWidget(goldText);

    yieldInfo->addWidget(prodLabel);
    yieldInfo->addWidget(prodText);

    yieldInfo->addWidget(foodLabel);
    yieldInfo->addWidget(foodText);

    yieldInfo->addWidget(sciLabel);
    yieldInfo->addWidget(sciText);

    yieldInfo->addWidget(culLabel);
    yieldInfo->addWidget(culText);

//    if(!this->isFullScreen())
//    {
        yieldInfo->addSpacerItem(new QSpacerItem(1000, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
//    }
//    else
//    {
//        float space = (static_cast<float>(widget.screenGeometry(widget.primaryScreen()).width()) / 1.2f) * 0.75f;
//        Yieldinfo->addSpacing(ceil(space));
//    }
        vLayout->insertLayout(0, yieldInfo);
}

void ClientManager::InitRenderData()
{
    renderer = new Renderer(map->GetMapSizeX(), thisPlayer);
    renderer->DrawHexScene(map, gameView);
    InitYieldDisplay();
    endGameText = new QString("Capitals Controlled:");

    for(int i = 0; i < civList.size(); i++)
    {
        renderer->LoadCities(civList.at(i)->GetCityList(), gameView);

        if(i == thisPlayer)
        {
            foreach(City* ci, civList.at(i)->GetCityList())
            {
                foreach(Tile* n, map->GetNeighborsRange(ci->GetCityTile(), 3))
                {
                    if(n->HasBeenDiscoveredBy(thisPlayer))
                    {
                        viewUpdateTiles->enqueue(ViewData{n->GetTileIndex(), thisPlayer, DISCOVERED});
                        renderer->SetTileTooltip(n->GetTileIndex(), *n->GetYield(), n->GetControllingCiv(), n->GetTileIDString());

                        if(!n->CanBeSeenBy(thisPlayer) && !n->ContainsUnit)
                        {
                            viewUpdateTiles->enqueue(ViewData{n->GetTileIndex(), thisPlayer, HIDDEN});
                        }
                    }
                    else
                    {
                        if(!n->HasBeenDiscoveredBy(thisPlayer))
                        {
                            n->DiscoverTile(thisPlayer);
                            n->SetAlwaysSeen(thisPlayer);
                            viewUpdateTiles->enqueue(ViewData{n->GetTileIndex(), thisPlayer, DISCOVERED});
                            renderer->SetTileTooltip(n->GetTileIndex(), *n->GetYield(), n->GetControllingCiv(), n->GetTileIDString());
                        }

                        if(!n->CanBeSeenBy(thisPlayer))
                        {
                            n->SeeTile(thisPlayer, true);
                            n->SetAlwaysSeen(thisPlayer);
                            viewUpdateTiles->enqueue(ViewData{n->GetTileIndex(), thisPlayer, VISIBLE});
                        }
                    }

                }
            }


            endGameText->append(QString("\nYou  1/%1").arg(civList.size()));
        }
        else
        {
            endGameText->append(QString("\n%1     1/%2").arg(civList.at(i)->GetLeaderName()).arg(civList.size()));
        }

        renderer->DrawUnits(civList.at(i)->GetUnitList(), map, gameView);

        for(int j = 0; j < civList.at(i)->GetCityList().size(); j++)
        {
            renderer->DrawCityBorders(civList.at(i)->GetCityAt(j), gameView, civList.at(i)->getCiv());
            if(i == thisPlayer)
            {
                if(j == 0)
                {
                    clv->addItem(civList.at(i)->GetCityAt(j)->GetName().append(" (").append(QChar(0x2605)).append(")"));
                }
                else
                {
                    clv->addItem(civList.at(i)->GetCityAt(j)->GetName());
                }

                foreach(Tile* tile, civList.at(i)->GetCityAt(j)->GetControlledTiles())
                {
                    renderer->SetTileWorkedIcon(tile, gameView);
                    renderer->SetTileTooltip(tile->GetTileIndex(), *tile->GetYield(), civList.at(i)->getCiv(), tile->GetTileIDString());
                }
            }

            civList.at(i)->GetCityAt(j)->loadUnits("Assets/Units/UnitList.txt");
            civList.at(i)->GetCityAt(j)->loadBuildings("Assets/Buildings/BuildingList.txt");
        }
    }

    endGameProgress->setText(*endGameText);

    ////Keep this statement. I need it at different points
    /// in the debugging process. -Port
//    renderer->DrawGridLines(gameView);
//    renderer->DrawGuiText(map, stringData, gameView);

    zoomScale = 1;

    for(int i = 0; i < 3; i++)
    {
        zoomIn();
    }

    gameView->centerOn(civList.at(thisPlayer)->GetCityAt(thisPlayer)->GetCityTile()->GetCenter());
    #ifndef __APPLE__
        playerInfoRect = new QRect(0, 0, this->width(), 20);
        gameStatusRect = new QRect(0, this->height() - 20, this->width(), 20);
    #endif
    statusMessage = " ";

    techLabel->setText(QString(" %1 ").arg(civList.at(thisPlayer)->getCurrentTech()->getName()));
    diplo->UpdateLeader(thisPlayer);
    renderReady = true;
}

void ClientManager::UpdateTiles()
{
    processedData = gameView->GetScene()->ProcessTile(relocateUnit);

//    if(processedData.newData || state == FOUND_CITY)
//    {
//        this->UpdateTileData();
//    }

//    TurnController();

    if(fortify && unitToMove != NULL && !unitToMove->isFortified)
    {
        fortify = false;
        unitToMove->isFortified = true;
        unitToMove->RequiresOrders = false;
        renderer->SetFortifyIcon(unitToMove->GetTileIndex(), false);
        renderer->SetUnitNeedsOrders(unitToMove->GetTileIndex(), false);
    }

    if(this->redrawTile && !selectedTileQueue->isEmpty())
    {
        this->redrawTile = false;
        renderer->UpdateScene(map, selectedTileQueue);
    }

    if(!viewUpdateTiles->isEmpty())
    {
        renderer->UpdateTileVisibilty(viewUpdateTiles, gameView);
    }

    this->update();

    if(!ns->HasNotificationsWaiting() && ns->item(0) == NULL)
    {
        ns->hide();
    }
    else if(ns->HasNotificationsWaiting())
    {
        if(ns->isHidden())
            ns->show();

        ns->ShowNotifications();
    }

    if(cityScreen != NULL && cityScreen->isHidden())
    {
        cityScreenVisible = true;
        showCity(NULL);
    }
}

void ClientManager::moveUnitTo()
{

}

void ClientManager::nextTurn()
{

}

void ClientManager::showTechTree()
{

}

void ClientManager::toggleDiplomacy()
{

}

void ClientManager::foundNewCity()
{

}

void ClientManager::buildNewRoad()
{

}

void ClientManager::buildNewFarm()
{

}

void ClientManager::buildNewPlantation()
{

}

void ClientManager::buildNewTradePost()
{

}

void ClientManager::buildNewMine()
{

}

void ClientManager::buildNewCamp()
{

}

void ClientManager::buildNewPasture()
{

}

void ClientManager::buildNewOilWell()
{

}

void ClientManager::buildNewFishBoat()
{

}

void ClientManager::buildNewQuarry()
{

}

void ClientManager::attackMelee()
{

}

void ClientManager::SetGoldFocus()
{

}

void ClientManager::SetProdFocus()
{

}

void ClientManager::SetScienceFocus()
{

}

void ClientManager::SetFoodFocus()
{

}

void ClientManager::SetCultureFocus()
{

}

void ClientManager::AttackCity()
{

}

void ClientManager::RangeAttack()
{

}

void ClientManager::Fortify()
{

}

void ClientManager::WarDeclared()
{

}

void ClientManager::WarAvoided()
{

}

void ClientManager::WarByInvasion()
{

}

void ClientManager::WarByDiplomacy()
{

}

void ClientManager::MakePeace()
{

}

void ClientManager::OpenHelp()
{

}

void ClientManager::parseItem()
{

}


