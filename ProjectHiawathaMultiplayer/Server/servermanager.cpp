/* ServerManager is the server-side GameManager.
 * This class is responsible for the following actions:
 *      - Turn Controller
 *      - AI turn processing
 *      - Start Turn
 *      - End Turn
 *      - Map Generation
 *      - Civ selection for civs listed as 'Random'
 *
 */

#include "servermanager.h"
#include <random>
#include <ctime>
#include <QTime>
#include <ui_servermanager.h>"

ServerManager::ServerManager(QWidget *parent, int mapX, int mapY, QVector<CivInfo*> civInfo) :
    QWidget(parent),
    ui(new Ui::ServerManager)
{
    ui->setupUi(this);
    qDebug() << "[ServerManager]" << "Starting Game Setup.";

    this->setWindowTitle("Project Hiawatha Server");
    InitServerVariables();

    map = new Map(mapX, mapY);
    map->InitHexMap();
    // Send data to clients
    map->WriteMapSaveData(obj);
    doc.setObject(obj);
    ServerHandler::instance()->SendClientUpdate(INITIAL_MAP_DATA, QString(doc.toJson()));
    qDebug() << "[ServerManager]" << "Initial map data sent";

    this->InitCivs(civInfo);

    foreach(Civilization *c, civList)
    {
        QJsonObject cObj;
        c->WriteData(cObj);
        arr.push_back(cObj);
    }

    obj.empty();
    obj["civs"] = arr;
    doc.setObject(obj);
    ServerHandler::instance()->SendClientUpdate(INITIAL_CIV_DATA, QString(doc.toJson()));

    qDebug() << "[ServerManager]" << "Initial civ data sent";

    arr.empty();
    obj.empty();

    map->WriteMapUpdateData(obj);
    doc.setObject(obj);
    ServerHandler::instance()->SendClientUpdate(MAP_UPDATE_DATA, QString(doc.toJson()));

    qDebug() << "[ServerManager]" << "Updated civ data sent";

    obj.empty();
    diplo->WriteDiploSaveData(obj);
    doc.setObject(obj);
    ServerHandler::instance()->SendClientUpdate(INITIAL_DIPLO_DATA, QString(doc.toJson()));

    qDebug() << "[ServerManager]" << "Initial diplo data sent";

//    mapInit = QtConcurrent::run(this->map, Map::InitHexMap);
//    civInit = QtConcurrent::run(this, ServerManager::InitCivs, civSelection, aiFlags);
//    civInit.waitForFinished();

    playersAliveCount = civList.size();
    currentTurn = 0;
    gameTurn = 0;

    //Start at 4000 BC. The game increments the turn to 1,
    // and subsequently the in game year by 40 years, upon game start.
    year = -4040;

    // Await ready response
}

void ServerManager::InitCivs(QVector<CivInfo*> ciVec)
{
    srand(time(0));
    QFile civData("Assets/Data/civ_data.json");
    if(!civData.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open civData file");
    }
    else
    {
        QByteArray byte = civData.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(byte);
        QJsonArray civRefData = doc.array();
        int civIndex = 0;
        QVector<int> selNat;

        foreach(CivInfo* ci, ciVec)
        {
            if(ci->nation != Random)
            {
                QJsonObject obj = civRefData.at((int)ci->nation).toObject();
                Civilization* civ = new Civilization(obj, ci->isAi);
                civ->loadTechs("Assets/Techs/Technology.txt");
                civ->setCurrentTech(civ->GetTechList().at(0));
                civ->setNextTech(civ->GetTechList().at(1));
                civ->setCivIndex(civIndex);

                diplo->AddLeader(obj["name"].toString(), QPixmap(obj["leaderimagepath"].toString()), civ->getCiv(), ci->isAi);

                civIndex++;

                civList.push_back(civ);
                selNat.push_back((int)ci->nation);
            }
            else
            {
                GenerateRandomCiv(civRefData, ci->isAi, selNat);
                civIndex++;
            }
        }
    }

//    if(!mapInit.isFinished())
//    {
//        mapInit.waitForFinished();
//    }

    map->SpawnCivs(civList);
}

void ServerManager::GenerateRandomCiv(QJsonArray &civRefData, bool ai, QVector<int> &selNat)
{
    int civNum;
    QJsonObject rObj;
    Civilization *civ;

newCivRand:
    // The modulo number at the end indicates the
    // max number of civs in the game (minus the random civ option).
    civNum = rand() % 16;

    // Look to see if the selected civ has already been chosen
    foreach(int j, selNat)
    {
        if(j == civNum)
        {
            goto newCivRand;
        }
    }

    // Civ has not already been chosen, so create a new civ and
    //  load it into the civList.
    rObj = civRefData.at(civNum).toObject();
    civ = new Civilization(rObj, ai);

    civ->loadTechs("Assets/Techs/Technology.txt");
    civ->setCurrentTech(civ->GetTechList().at(0));
    civ->setNextTech(civ->GetTechList().at(1));
    civ->setCivIndex(civList.size());
    civList.push_back(civ);
    diplo->AddLeader(rObj["name"].toString(), QPixmap(rObj["leaderimagepath"].toString()), (Nation)civNum, ai);

    selNat.push_back(civNum);

}

void ServerManager::LoadCivs()
{

}

void ServerManager::TurnController()
{
    static bool AiTurnInProgress = false;
    //Is it currently the Player's turn?
    if(currentTurn == 0)
    {
        //Has the player ended their turn?
        if(turnEnded)
        {
            turnEnded = false;
//            EndTurn();

        }
        else if(turnStarted)
        {
            turnStarted = false;
//            StartTurn();
            ServerHandler::instance()->SendClientUpdate(TURN_START, QString("%1").arg(currentTurn));
        }
    }
    else
    {
        //Cycle through the AI turns.
        if(civList.at(currentTurn)->HasCivMetPlayer())
            statusMessage = QString("--------<< Processing Turn for %1 >>--------").arg(civList.at(currentTurn)->GetLeaderName());
        else
            statusMessage = QString("--------<< Processing Turn for Unmet Player %1 >>--------").arg(currentTurn);

        if(!AiTurnInProgress)
        {
//            StartTurn();

#ifdef __APPLE__
            this->ac->turnStarted(civList.at(currentTurn),civList.at(0),this->map);
            while(!civList.at(currentTurn)->isEmpty())
            {
                AIQueueData data = civList.at(currentTurn)->dequeue();
                state = data.action;

                if(state == AI_FOUND_CITY)
                {
                    unitToMove = data.unit;
                }
                else if(state == CONQUER)
                {
                    state = FIND_CITY;
                    unitToMove = data.unit;
                    targetTile = map->GetTileAt(data.unit->GetTargetTileIndex());
                }
                else if(state == AI_DECLARE_WAR)
                {
                    diplo->UpdateLeader(0);
                    WarByDiplomacy();
                }

                this->UpdateTileData();
            }
#else
            QFuture<void> future = QtConcurrent::run(this->ac, AI_Controller::turnStarted, civList.at(currentTurn), civList.at(0), this->map);
            //While the AI is taking its turn, check the Queue
            // if the AI is trying to settle a city or conquer one
            while(future.isRunning())
            {
                if(!civList.at(currentTurn)->isEmpty())
                {
                    AIQueueData data = civList.at(currentTurn)->dequeue();
                    state = data.action;

                    if(state == AI_FOUND_CITY)
                    {
                        unitToMove = data.unit;
                    }
                    else if(state == CONQUER)
                    {
                        state = FIND_CITY;
                        unitToMove = data.unit;
                        targetTile = map->GetTileAt(data.unit->GetTargetTileIndex());
                    }
                    else if(state == AI_DECLARE_WAR)
                    {
                        diplo->UpdateLeader(0);
                        WarByDiplomacy();
                    }
                    else if(state == ATTACK_MELEE)
                    {
                        unitToMove = data.unit;
                        targetUnit = data.target;
                        ProcessAttackUnit();
                    }
                    else if(state == ATTACK_RANGE)
                    {
                        unitToMove = data.unit;
                        targetUnit = data.target;
                        ProcessAttackUnit();
                    }

                    this->UpdateTileData();
                }
            }
            //This is just for extra precautions so that we don't try to end the AI's
            // turn and move on before the AI thread has completly finished.
            future.waitForFinished();
#endif
//            EndTurn();
            AiTurnInProgress = false;
        }
    }
}

void ServerManager::Defeat()
{

}

void ServerManager::Victory()
{

}

void ServerManager::LoadJsonData()
{
    QFile buildExport("Assets/Data/buildings.json");

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

    QFile unitExport("Assets/Data/units.json");

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

    QFile techExport("Assets/Data/techList.json");

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

void ServerManager::UpdateTileData()
{
//   if(!processedData.relocateOrderGiven && state == IDLE)
//   {
//       unitTile = map->GetTileFromCoord(processedData.column, processedData.row);

//       if(unitTile->Selected)
//       {
//           unitTile->Selected = false;
//           selectedTileQueue->enqueue(SelectData{unitTile->GetTileIndex(), false, false});
//           this->redrawTile = true;
//       }

//       if(unitTile->ContainsUnit)
//       {
//           state = FIND_UNIT;
//       }
//   }
//   else if(state == ATTACK_MELEE || processedData.relocateOrderGiven || state == ATTACK_RANGE || state == ATTACK_CITY)
//   {
//       targetTile = map->GetTileFromCoord(processedData.column, processedData.row);

//       if((targetTile->ContainsUnit || targetTile->HasCity) && (targetTile->GetControllingCivListIndex() != 0) && (targetTile->GetControllingCivListIndex() != -1))
//       {
//           if(uc->AtPeaceWith(targetTile, WarData{civList.at(currentTurn)->isAtWar(), civList.at(currentTurn)->GetCivListIndexAtWar()}))
//           {
//               if(gameTurn == 1)
//               {
//                   statusMessage = "--------<< You cannot declare war on the first turn. >>--------";
//                   state = IDLE;
//               }
//               else
//               {
//                   warbox->setText(QString("You are not at war with %1.\nIf you continue, this will be a declaration of war. \nContinue?").arg(civList.at(targetTile->GetControllingCivListIndex())->GetLeaderName()));

//                   warbox->setWindowFlags(Qt::FramelessWindowHint);

//                   connect(warbox->button(QMessageBox::Ok), SIGNAL(clicked(bool)), this, SLOT(WarDeclared()));
//                   connect(warbox->button(QMessageBox::Cancel), SIGNAL(clicked(bool)), this, SLOT(WarAvoided()));

//                   warbox->exec();

//                   disconnect(warbox->button(QMessageBox::Ok), SIGNAL(clicked(bool)), this, SLOT(WarDeclared()));
//                   disconnect(warbox->button(QMessageBox::Cancel), SIGNAL(clicked(bool)), this, SLOT(WarAvoided()));
//               }
//           }
//       }

//       if(!targetTile->ContainsUnit  && (state == ATTACK_MELEE || state == ATTACK_RANGE))
//       {
//           QList<Tile*> neighbors = map->GetNeighbors(targetTile);

//           foreach(Tile* tile, neighbors)
//           {
//               if(tile->ContainsUnit && !tile->HasCity)
//               {
//                   if(tile->GetOccupyingCivListIndex() != currentTurn)
//                   {
//                       if(!uc->AtPeaceWith(tile, WarData{civList.at(currentTurn)->isAtWar(), civList.at(currentTurn)->GetCivListIndexAtWar()}))
//                       {
//                           targetTile = tile;
//                           break;
//                       }
//                   }
//               }
//           }
//       }

//       if(state == ATTACK_CITY)
//       {
//           state = FIND_CITY;
//       }
//   }

//   if(state == FIND_UNIT)
//   {
//       state = IDLE;

//       if(unitToMove != NULL)
//       {
//           selectedTileQueue->enqueue(SelectData{unitToMove->GetTileIndex(), false, false});
//       }

//       unitToMove = uc->FindUnitAtTile(unitTile, civList.at(currentTurn)->GetUnitList());

//       selectedTileQueue->enqueue(SelectData{unitToMove->GetTileIndex(), true, false});
//       tileModifiedQueue->enqueue(SelectData{unitToMove->GetTileIndex(), false, false});

//       if(unitToMove->GetOwner() == civList.at(currentTurn)->getCiv() && (unitToMove->RequiresOrders || unitToMove->isFortified))
//       {
//           if(unitToMove->isFortified)
//           {
//               renderer->SetFortifyIcon(unitToMove->GetTileIndex(), true);
//               unitToMove->isFortified = false;
//           }

//           map->GetTileAt(unitToMove->GetTileIndex())->Selected = true;
//           moveUnit->setEnabled(true);
//           this->redrawTile = true;

//           if(unitToMove->isNonCombat())
//           {
//               attackUnit->setEnabled(false);
//               attackCity->setEnabled(false);
//               rangeAttack->setEnabled(false);
//               fortifyUnit->setEnabled(false);

//               if(unitToMove->GetUnitType() == SETTLER)
//               {
//                   if(!map->GetTileAt(unitToMove->GetTileIndex())->HasCity)
//                   {
//                       foundCity->setEnabled(true);

//                       buildFarm->setEnabled(false);
//                       buildMine->setEnabled(false);
//                       buildPlantation->setEnabled(false);
//                       buildTradePost->setEnabled(false);
//                       buildRoad->setEnabled(false);
//                       buildCamp->setEnabled(false);
//                       buildPasture->setEnabled(false);
//                       buildFishBoat->setEnabled(false);
//                       buildOilWell->setEnabled(false);
//                       buildQuarry->setEnabled(false);
//                   }
//               }
//               else if (unitToMove->GetUnitType() == WORKER)
//               {
//                   if(map->GetTileAt(unitToMove->GetTileIndex())->GetControllingCiv() == civList.at(currentTurn)->getCiv())
//                   {
//                       if(map->GetTileAt(unitToMove->GetTileIndex())->CanHaveFarm)
//                           buildFarm->setEnabled(true);

//                       if(map->GetTileAt(unitToMove->GetTileIndex())->CanHaveMine)
//                           buildMine->setEnabled(true);

//                       if(map->GetTileAt(unitToMove->GetTileIndex())->CanHavePlantation)
//                           buildPlantation->setEnabled(true);

//                       if(map->GetTileAt(unitToMove->GetTileIndex())->CanHaveTrade)
//                           buildTradePost->setEnabled(true);

//                       if(map->GetTileAt(unitToMove->GetTileIndex())->CanHaveCamp)
//                           buildCamp->setEnabled(true);

//                       if(map->GetTileAt(unitToMove->GetTileIndex())->CanHaveFishBoat)
//                           buildFishBoat->setEnabled(true);

//                       if(map->GetTileAt(unitToMove->GetTileIndex())->CanHaveOilWell)
//                           buildOilWell->setEnabled(true);

//                       if(map->GetTileAt(unitToMove->GetTileIndex())->CanHavePasture)
//                           buildPasture->setEnabled(true);

//                       if(map->GetTileAt(unitToMove->GetTileIndex())->CanHaveQuarry)
//                           buildQuarry->setEnabled(true);
//                   }

//                   foundCity->setEnabled(false);

//                   buildRoad->setEnabled(true);
//               }
//           }
//           else //Combat Unit button controls
//           {
//               attackCity->setEnabled(false);
//               attackUnit->setEnabled(false);
//               rangeAttack->setEnabled(false);
//               foundCity->setEnabled(false);
//               buildFarm->setEnabled(false);
//               buildMine->setEnabled(false);
//               buildPlantation->setEnabled(false);
//               buildTradePost->setEnabled(false);
//               buildRoad->setEnabled(false);
//               buildCamp->setEnabled(false);
//               buildPasture->setEnabled(false);
//               buildFishBoat->setEnabled(false);
//               buildOilWell->setEnabled(false);
//               buildQuarry->setEnabled(false);

//               fortifyUnit->setEnabled(true);

//               QList<Tile*> tiles = map->GetNeighborsRange(unitTile, unitToMove->GetRange());

//               foreach(Tile *tile, tiles)
//               {
//                   if(((tile->GetOccupyingCivListIndex() > 0) || (tile->GetControllingCivListIndex() > 0)) && (tile->HasCity || tile->ContainsUnit))
//                   {
//                       int tileIndex = tile->GetTileIndex();

//                       if(tile->HasCity && tile->GetControllingCivListIndex() != 0)
//                       {
//                           qDebug() << "   Tile:" << tile->GetTileIDString() << "controlled by:" << tile->GetControllingCivListIndex() << "occupied by" << tile->GetOccupyingCivListIndex();
//                           selectedTileQueue->enqueue(SelectData {tileIndex, false, true});
//                           tileModifiedQueue->enqueue(SelectData {tileIndex, false, false});
//                           attackCity->setEnabled(true);
//                       }
//                       else if(tile->ContainsUnit && !tile->HasCity && tile->GetOccupyingCivListIndex() != 0)
//                       {
//                           selectedTileQueue->enqueue(SelectData {tileIndex, false, true});
//                           tileModifiedQueue->enqueue(SelectData {tileIndex, false, false});

//                           if(unitToMove->isMelee)
//                           {
//                               attackUnit->setEnabled(true);
//                           }
//                           else if(!unitToMove->isMelee)
//                           {
//                               attackUnit->setEnabled(false);
//                               rangeAttack->setEnabled(true);
//                           }
//                       }
//                   }
//               }
//           }
//       }
//       else
//       {
//           if(currentTurn == 0)
//           {
//               if(unitToMove->GetOwner() != civList.at(0)->getCiv())
//                   statusMessage = "--------<< You do not own that unit >>--------";
//               else
//                   statusMessage = "--------<< You cannot give orders to this unit >>-------";
//           }

//           selectedTileQueue->enqueue(SelectData{unitToMove->GetTileIndex(), false, false});
//       }
//   }
//   else if(state == FIND_CITY)
//   {
//       state = IDLE;
//       targetCity = uc->FindCityAtTile(targetTile, civList.at(targetTile->GetControllingCivListIndex())->GetCityList());

//       if(currentTurn == 0)
//       {
//           selectedTileQueue->enqueue(SelectData{unitToMove->GetTileIndex(), false, false});
//           selectedTileQueue->enqueue(SelectData{targetCity->GetCityTile()->GetTileIndex(), false, false});
//           attackCity->setEnabled(false);
//       }

//       if(!uc->AtPeaceWith(targetCity->GetCityTile(), WarData{civList.at(currentTurn)->isAtWar(), civList.at(currentTurn)->GetCivListIndexAtWar()}))
//       {
//           uc->AttackCity(unitToMove, targetCity);

//           //City Conquering Logic
//           if(targetCity->GetCityHealth() <= 0 && unitToMove->isMelee)
//           {
//               if(currentTurn == 0)
//                   statusMessage = QString("--------<< %1 Has Been Conquered! >>--------").arg(targetCity->GetName());

//               int targetIndex = targetTile->GetControllingCivListIndex();

//               ProcessCityConquer(targetCity, civList.at(currentTurn), civList.at(targetIndex));

//               if(civList.at(targetIndex)->GetCityList().size() == 0)
//               {
//                   if(!civList.at(targetIndex)->GetUnitList().isEmpty())
//                   {
//                       foreach(Unit* unit, civList.at(targetIndex)->GetUnitList())
//                       {
//                           map->GetTileAt(unit->GetTileIndex())->ContainsUnit = false;
//                           map->GetTileAt(unit->GetTileIndex())->SetOccupyingCivListIndex(-1);
//                           renderer->RemoveUnit(unit, gameView);
//                       }

//                       for(int i = 0; i < civList.at(targetIndex)->GetUnitList().size(); i++)
//                       {
//                           civList.at(targetIndex)->RemoveUnit(i);
//                       }
//                   }
//                   playersAliveCount--;
//               }
//           }

//           renderer->UpdateUnits(map, gameView, unitToMove, false);
//           renderer->UpdateCityHealthBar(targetCity, gameView);
//           renderer->SetUnitNeedsOrders(unitToMove->GetTileIndex(), false);
//           this->redrawTile = true;
//       }
//   }

//   if(processedData.relocateOrderGiven && state == MOVE_UNIT)
//   {
//       if(uc->AtPeaceWith(targetTile, WarData{civList.at(currentTurn)->isAtWar(), civList.at(currentTurn)->GetCivListIndexAtWar()})
//               && unitToMove->GetOwner() != targetTile->GetControllingCiv())
//       {
//           if(gameTurn == 1)
//           {
//               statusMessage = "--------<< You cannot declare war on the first turn. >>--------";
//               state = IDLE;
//               relocateUnit = false;
//           }
//           else
//           {
//               if(targetTile->GetOccupyingCivListIndex() == -1)
//                   warbox->setText(QString("You are not at war with %1.\nIf you continue, this will be a declaration of war. \nContinue?").arg(civList.at(targetTile->GetControllingCivListIndex())->GetLeaderName()));
//               else
//                   warbox->setText(QString("You are not at war with %1.\nIf you continue, this will be a declaration of war. \nContinue?").arg(civList.at(targetTile->GetOccupyingCivListIndex())->GetLeaderName()));

//               warbox->setWindowFlags(Qt::FramelessWindowHint);

//               connect(warbox->button(QMessageBox::Ok), SIGNAL(clicked(bool)), this, SLOT(WarByInvasion()));
//               connect(warbox->button(QMessageBox::Cancel), SIGNAL(clicked(bool)), this, SLOT(WarAvoided()));

//               warbox->exec();

//               disconnect(warbox->button(QMessageBox::Ok), SIGNAL(clicked(bool)), this, SLOT(WarByInvasion()));
//               disconnect(warbox->button(QMessageBox::Cancel), SIGNAL(clicked(bool)), this, SLOT(WarAvoided()));
//           }
//       }
//       else
//       {
//           if(targetTile->ContainsUnit)
//           {
//               statusMessage = "--------<< You cannot move there >>--------";
//               state = IDLE;
//           }
//           else
//           {
//               //Even though we aren't technically invading all the time,
//               //  this is set to simply allow the move processing to occur
//               state = INVADE;
//           }
//       }

//       if(state == INVADE)
//       {
//           state = IDLE;

//           unitToMove->SetUnitTargetTile(targetTile->GetTileID().column, targetTile->GetTileID().row);
//           unitToMove->SetUnitTargetTileIndex(targetTile->GetTileIndex());

//           if(unitToMove->isFortified)
//           {
//               unitToMove->isFortified = false;
//               renderer->SetFortifyIcon(unitToMove->GetTileIndex(), true);
//           }

//           uc->FindPath(unitTile, targetTile, map, unitToMove, WarData{civList.at(currentTurn)->isAtWar(), civList.at(currentTurn)->GetCivListIndexAtWar()});

//           relocateUnit = false;
//           processedData.relocateOrderGiven = false;
//           map->GetTileAt(unitToMove->GetTileIndex())->Selected = false;
//           renderer->SetUnitNeedsOrders(unitToMove->GetTileIndex(), false);
//           selectedTileQueue->enqueue(SelectData{unitToMove->GetTileIndex(), false, false});
//           this->redrawTile = true;
//       }

//       foundCity->setEnabled(false);

//       buildFarm->setEnabled(false);
//       buildMine->setEnabled(false);
//       buildPlantation->setEnabled(false);
//       buildTradePost->setEnabled(false);
//       buildRoad->setEnabled(false);
//       buildCamp->setEnabled(false);
//       buildPasture->setEnabled(false);
//       buildFishBoat->setEnabled(false);
//       buildOilWell->setEnabled(false);
//       buildQuarry->setEnabled(false);

//       attackUnit->setEnabled(false);
//       attackCity->setEnabled(false);
//       rangeAttack->setEnabled(false);
//       fortifyUnit->setEnabled(false);
//   }

//   if(state == ATTACK_MELEE || state == ATTACK_RANGE)
//   {
//       state = IDLE;
//       ProcessAttackUnit();
//   }

//   //City founding Logic. This operates nearly the same as
//   // initial city spawning in the Map class
//   if(state == FOUND_CITY || state == AI_FOUND_CITY)
//   {
//       state = IDLE;
//       foundCityIndex = unitToMove->GetTileIndex();
//       City* city = map->CreateCity(foundCityIndex, civList.at(currentTurn), false);

//       qDebug() << "   Checking spawn location";
//       bool valid = true;
//       int dstX, dstY;
//       for(int i = 0; i < civList.size(); i++)
//       {
//           foreach(City* existingCity, civList.at(i)->GetCityList())
//           {
//               dstX = city->GetCityTile()->GetTileID().column - existingCity->GetCityTile()->GetTileID().column;
//               dstY = city->GetCityTile()->GetTileID().row - existingCity->GetCityTile()->GetTileID().row;

//               if((abs(dstX) < 8 && dstY == 0) || (abs(dstX) <= 4 && abs(dstY) <= 4) || (dstX == 0 && abs(dstY) < 4))
//               {
//                   if(currentTurn == 0)
//                       statusMessage = "--------<< You cannot settle this close to another city. >>--------";

//                   civList.at(currentTurn)->SetCityIndex(civList.at(currentTurn)->getCityIndex() - 1);
//                   foreach(Tile* tile, city->GetControlledTiles())
//                   {
//                       tile->SetControllingCiv(NO_NATION, -1);
//                   }
//                   delete city;
//                   unitToMove->RequiresOrders = true;
//                   civList.at(currentTurn)->cityFounded= false;
//                   valid = false;
//                   break;
//               }
//           }

//           if(!valid)
//               break;
//       }

//       if(!valid)
//           return;

//       qDebug() << "City spawn valid";
//       city->loadBuildings("Assets/Buildings/BuildingList.txt");
//       city->loadUnits("Assets/Units/UnitList.txt");
//       civList.at(currentTurn)->AddCity(city);
//       map->GetTileAt(foundCityIndex)->HasCity = true;

//       QList<Tile*> cityMEB = map->GetNeighborsRange(city->GetCityTile(), 4);
//       city->SetMaximumExpansionBorderTiles(cityMEB);
//       city->FilterMEBList();

//       map->GetTileQueue(city);
//       map->DefineCityBordersNew(city);
////        city->SortTileQueue();

//       city->SortControlledTiles();
//       city->GetControlledTiles().first()->IsWorked = true;

//       renderer->AddCity(city, gameView, false);

//       renderer->DrawCityBorders(city, gameView, civList.at(currentTurn)->getCiv());

//       if(currentTurn == 0)
//       {
//           foreach(Tile* tile, city->GetControlledTiles())
//           {
//               renderer->SetTileWorkedIcon(tile, gameView);
//           }

//           renderer->SetFortifyIcon(foundCityIndex, true);
//           renderer->SetUnitNeedsOrders(foundCityIndex, false);
//       }
//       civList.at(currentTurn)->UpdateCivYield();

//       map->GetTileAt(foundCityIndex)->ContainsUnit = false;
//       map->GetTileAt(foundCityIndex)->HasCity=true;
//       map->GetTileAt(foundCityIndex)->SetControllingCiv(civList.at(currentTurn)->getCiv(), currentTurn);

//       renderer->RemoveUnit(unitToMove, gameView);
//       civList.at(currentTurn)->RemoveUnit(unitToMove->GetUnitListIndex());

//       if(currentTurn == 0)
//       {
//           clv->addItem(city->GetName());
//           selectedTileQueue->enqueue(SelectData{foundCityIndex, false, false});
//           foundCity->setEnabled(false);
//       }
//   }

//   if((map->GetTileFromCoord(processedData.column, processedData.row)->Selected == false) && state != AI_FOUND_CITY)
//   {
//       if(unitToMove != NULL)
//       {
//           selectedTileQueue->enqueue(SelectData{unitToMove->GetTileIndex(), false, false});
//           if(!tileModifiedQueue->isEmpty())
//           {
//               renderer->UpdateScene(map, gameView, tileModifiedQueue);
//           }
//       }

//       attackCity->setEnabled(false);
//       attackUnit->setEnabled(false);
//       rangeAttack->setEnabled(false);
//       buildFarm->setEnabled(false);
//       buildMine->setEnabled(false);
//       buildPlantation->setEnabled(false);
//       buildTradePost->setEnabled(false);
//       buildRoad->setEnabled(false);
//       buildCamp->setEnabled(false);
//       buildPasture->setEnabled(false);
//       buildFishBoat->setEnabled(false);
//       buildOilWell->setEnabled(false);
//       buildQuarry->setEnabled(false);
//       moveUnit->setEnabled(false);
//       fortifyUnit->setEnabled(false);
//       redrawTile = true;
//   }
}

void ServerManager::InitServerVariables()
{
    ac = new AI_Controller();
    uc = new UnitController();
//    about = new About();
    diplo = new Diplomacy(this);
    diplo->hide();

//    techTree = new TechTree(this);

//    selectedTileQueue = new QQueue<SelectData>();
//    tileModifiedQueue = new QQueue<SelectData>();
//    viewUpdateTiles = new QQueue<ViewData>();

    unitToMove = NULL;
    targetUnit = NULL;
    targetCity = NULL;
    unitTile = NULL;
    targetTile = NULL;
    state = IDLE;

//    cityScreenVisible = false;
//    techTreeVisible = false;
//    diploVisible = false;
//    toggleOn = false;
//    relocateUnit = false;
    turnEnded = false;
    turnStarted = true;
//    countTime = false;
//    citySelected = false;
//    updateFoW = false;
//    focusChanged = false;
//    fortify = false;

//    currentProductionName = "No Production Selected";

    LoadJsonData();

//    updateTimer = new QTimer();
//    updateTimer->setInterval(17);
//    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTiles()));
//    updateTimer->start();

    qsrand(QTime::currentTime().msec());
//    this->InitButtons();
//    this->InitLayouts();
//    this->setLayout(vLayout);

    #ifdef __APPLE__
        playerInfoRect = new QRect(0, 0, this->width(), 20);
        gameStatusRect = new QRect(0, this->height() - 20, this->width(), 20);
    #endif

//    this->show();
//        ns->hide();
}

void ServerManager::ProcessCityConquer(City *tCity, Civilization *aCiv, Civilization *tCiv)
{

}

void ServerManager::ProcessAttackUnit()
{

}

void ServerManager::ProcessPeace(int makePeaceWithIndex)
{

}

bool ServerManager::AcceptsPeace(Civilization *ai)
{

}

void ServerManager::WarDeclared()
{

}

void ServerManager::WarAvoided()
{

}

void ServerManager::WarByInvasion()
{

}

void ServerManager::WarByDiplomacy()
{

}

void ServerManager::MakePeace()
{

}

void ServerManager::on_exitServerPB_clicked()
{
//    ServerHandler::instance()->StopServer();
    close();
}
