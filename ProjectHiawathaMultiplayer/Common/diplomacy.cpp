#include "diplomacy.h"
#include "unitcontroller.h"

#include <QMessageBox>
#include <QDebug>
#include <QJsonArray>

Diplomacy::Diplomacy(QWidget *parent) : QWidget(parent)
{
    QString DiploStyle = "QWidget { background-color: #145e88; } QFrame { background-color: black; }";
    DiploStyle += "QPushButton { background-color: #4899C8; border: 1px solid #f6b300; border-radius: 6px; font: 10px; min-width: 100px;}";
    DiploStyle += "QPushButton:pressed { background-color: #77adcb; }";
    DiploStyle += "QScrollBar:vertical { border: 2px sold black; background: #77adcb; width: 15px; margin: 12px 0 12px 0;} QScrollBar::handle:vertical { background: #4899C8; min-height: 10px; }";
    DiploStyle += "QScrollBar::add-line:vertical { border: 1px solid black; background: #dedede; height: 10px; subcontrol-position: bottom; subcontrol-origin: margin; }  QScrollBar::sub-line:vertical { border: 1px solid black; height: 10px; background: #dedede; subcontrol-position: top; subcontrol-origin: margin; }";
    DiploStyle += "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { border: 1px solid black; width: 3px; height: 3px; background: purple; } QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }";
    DiploStyle += "QListView { background-color: grey; font-size: 24; color: black; border: 5px inset #f6b300; show-decoration-selected: 1; } QLabel{ border: 2px inset #f6b300; background: #dedede; color: black; font: bold; max-height: 15px; min-width: 150px; text-align: center; }";

    this->setStyleSheet(DiploStyle);
    leaderListArea = new QListWidget(this);
    leaderImage = new QLabel("?");

    this->setFixedSize(1000, 500);
    leaderListArea->setViewMode(QListWidget::ListMode);
    leaderListArea->setAcceptDrops(false);
    leaderListArea->setMaximumSize(650, 500);

    leaderName = new QLabel(" ");
    warStatus = new QLabel(" ");
    nationName = new QLabel(" ");
    atWarWith = new QLabel(" ");
    atWarWith->setStyleSheet("QLabel{ border: 2px inset #f6b300; background: #dedede; color: red; font: bold; min-height: 50px; max-height: 100px; min-width: 150px; }");
    turn = 0;

    makePeace = new QPushButton("Make Peace");
    makePeace->setEnabled(false);

    declareWar = new QPushButton("Declare War");
    declareWar->setEnabled(true);

    closeDiplo = new QPushButton("Return to game");
    connect(closeDiplo, SIGNAL(clicked(bool)), this, SLOT(hide()));

    connect(leaderListArea, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectLeader(QListWidgetItem*)));

    QHBoxLayout *civInfo = new QHBoxLayout();
    civInfo->addWidget(leaderName);
    civInfo->addWidget(nationName);

    QVBoxLayout *overview = new QVBoxLayout();
    overview->addWidget(leaderImage);
    overview->addLayout(civInfo);
    overview->addWidget(atWarWith);

    QHBoxLayout *warButtons = new QHBoxLayout();
    warButtons->addWidget(declareWar);
    warButtons->addWidget(makePeace);
    overview->addLayout(warButtons);

    QVBoxLayout *list = new QVBoxLayout();
    list->addWidget(leaderListArea);
    list->addWidget(closeDiplo);

    QHBoxLayout *window = new QHBoxLayout();
    window->addLayout(list);
    window->addLayout(overview);

    this->setLayout(window);
}

Diplomacy::~Diplomacy()
{
    foreach(DiplomacyItem* di, diploItemList)
    {
        if(di != NULL)
            delete di;
    }

    delete closeDiplo;
    delete leaderImage;
    delete leaderName;
    delete warStatus;
    delete nationName;
    delete atWarWith;
    delete leaderListArea;

    delete declareWar;
    delete makePeace;
}

void Diplomacy::SetLocalPlayer(Nation player, int civListIndex)
{
    localPlayer = player;
    localPlayerIndex = civListIndex;
}

void Diplomacy::UpdateTurn()
{
    turn++;
}

void Diplomacy::UpdateLeader(int row)
{
    leaderListArea->setCurrentRow(row);
}

void Diplomacy::MeetPlayer(int row, int playerIndex)
{
    WarHistory wh;
    wh.hasMet = true;
    wh.nation = diploItemList.at(row)->warChart.at(playerIndex).nation;
    wh.timesAtWarWith = diploItemList.at(row)->warChart.at(playerIndex).timesAtWarWith;
    wh.warStartedOn = diploItemList.at(row)->warChart.at(playerIndex).warStartedOn;
    wh.warStat = diploItemList.at(row)->warChart.at(playerIndex).warStat;

    diploItemList.at(row)->warChart.replace(playerIndex, wh);
    diploItemList.at(row)->displayString = QString("%1 \t(%2)\n").arg(diploItemList.at(row)->leaderName).arg(UnitController::NationName(diploItemList.at(row)->nation));
    leaderListArea->takeItem(row);
    leaderListArea->insertItem(row, diploItemList.at(row)->displayString);
}

void Diplomacy::SetLeaderImage(int index, QPixmap &image)
{
    diploItemList.at(index)->image = image;
}

void Diplomacy::WriteDiploSaveData(QJsonObject &obj) const
{
    QJsonArray dArray;

    foreach(DiplomacyItem *di, diploItemList)
    {
        QJsonObject dio;
        dio["nation"] = di->nation;
        dio["leader"] = di->leaderName;
        dio["displaystring"] = di->displayString;

        QJsonArray whArray;
        foreach(WarHistory wh, di->warChart)
        {
            QJsonObject who;
            who["targetnation"] = wh.nation;
            who["timesatwarwith"] = wh.timesAtWarWith;
            who["warstartedon"] = wh.warStartedOn;
            who["warstatus"] = wh.warStat;
            who["hasmet"] = wh.hasMet;
            whArray.append(who);
        }

        dio["warchart"] = whArray;
        dArray.append(dio);
    }

    obj["diplomacyitems"] = dArray;
}

void Diplomacy::ReadDiploSaveData(const QJsonObject &obj)
{
    QJsonArray dArray = obj["diplomacyitems"].toArray();
    DiplomacyItem *di;
    for(int i = 0; i < dArray.size(); i++)
    {
        QJsonObject dio = dArray.at(i).toObject();
        di = new DiplomacyItem{QPixmap(), " ", QString(" "), NO_NATION};
        di->nation = static_cast<Nation>(dio["nation"].toInt());
        di->leaderName = dio["leader"].toString();
        di->displayString = dio["displaystring"].toString();

        QJsonArray whArray = dio["warchart"].toArray();
        for(int j = 0; j < whArray.size(); j++)
        {
            QJsonObject who = whArray.at(j).toObject();
            WarHistory wh{NO_NATION, 0, 0, AT_PEACE};
            wh.nation = static_cast<Nation>(who["targetnation"].toInt());
            wh.timesAtWarWith = who["timesatwarwith"].toInt();
            wh.warStartedOn = who["warstartedon"].toInt();
            wh.warStat = static_cast<WarStatus>(who["warstatus"].toInt());
            wh.hasMet = who["hasmet"].toBool();

            di->warChart.push_back(wh);
        }

        leaderListArea->addItem(di->displayString);
        diploItemList.push_back(di);
    }
}

void Diplomacy::WriteDiploUpdateData(QJsonObject &obj) const
{
    QJsonArray dArr;
    DiplomacyItem *di;
    foreach(UpdateStatus us, updatedDiploItems)
    {
        di = diploItemList.at(us.diIndex);

        QJsonObject dObj;
        dObj["diploindex"] = us.diIndex;

        QJsonObject dio;
        dio["nation"] = di->nation;
        dio["leader"] = di->leaderName;
        dio["displaystring"] = di->displayString;

        QJsonArray whArray;
        foreach(WarHistory wh, di->warChart)
        {
            QJsonObject who;
            who["targetnation"] = wh.nation;
            who["timesatwarwith"] = wh.timesAtWarWith;
            who["warstartedon"] = wh.warStartedOn;
            who["warstatus"] = wh.warStat;
            who["hasmet"] = wh.hasMet;
            whArray.append(who);
        }

        dObj["updates"] = dio;

        dArr.push_back(dObj);
    }

    obj["updates"] = dArr;
}

void Diplomacy::ReadDiploUpdateData(const QJsonObject &obj)
{
    QJsonArray dArr = obj["updates"].toArray();
    DiplomacyItem *di;

    for(int i = 0; i < dArr.size(); i++)
    {
        QJsonObject dObj = dArr.at(i).toObject();
        QJsonObject dio = dObj["updates"].toObject();
        QJsonArray whArray = dio["warchart"].toArray();

        di = diploItemList.at(dObj["diploindex"].toInt());

        di->nation = static_cast<Nation>(dio["nation"].toInt());
        di->leaderName = dio["leader"].toString();
        di->displayString = dio["displaystring"].toString();

        for(int j = 0; j < whArray.size(); j++)
        {
            QJsonObject who = whArray.at(j).toObject();
            WarHistory wh{NO_NATION, 0, 0, AT_PEACE};
            wh.nation = static_cast<Nation>(who["targetnation"].toInt());
            wh.timesAtWarWith = who["timesatwarwith"].toInt();
            wh.warStartedOn = who["warstartedon"].toInt();
            wh.warStat = static_cast<WarStatus>(who["warstatus"].toInt());
            wh.hasMet = who["hasmet"].toBool();

            di->warChart.replace(j, wh);
        }
    }
}

int Diplomacy::GetLengthOfWar(Nation ai)
{
    for(int i = 0; i < diploItemList.at(localPlayerIndex)->warChart.size(); i++)
    {
        if(diploItemList.at(localPlayerIndex)->warChart.at(i).nation == ai)
        {
            return (turn - diploItemList.at(localPlayerIndex)->warChart.at(i).warStartedOn);
        }
    }
}

int Diplomacy::GetNumberOfWars(Nation ai)
{
    for(int i = 0; i < diploItemList.at(localPlayerIndex)->warChart.size(); i++)
    {
        if(diploItemList.at(localPlayerIndex)->warChart.at(i).nation == ai)
        {
            return diploItemList.at(localPlayerIndex)->warChart.at(i).timesAtWarWith;
        }
    }
}

bool Diplomacy::AtPermanentWar(Nation ai)
{
    for(int i = 0; i < diploItemList.at(localPlayerIndex)->warChart.size(); i++)
    {
        if(diploItemList.at(localPlayerIndex)->warChart.at(i).nation == ai)
        {
            if(diploItemList.at(localPlayerIndex)->warChart.at(i).warStat == PERMANENT_WAR)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

int Diplomacy::GetIndex()
{
    return leaderListArea->currentRow();
}

void Diplomacy::AddLeader(QString _name, QPixmap _image, Nation _nation, bool isPlayer)
{
    DiplomacyItem *di = new DiplomacyItem{_image, _name, QString(" "), _nation};
    if(!isPlayer)
    {
        WarHistory wh{_nation, 0, 0, AT_PEACE, false};

        //Add new leader WarHistory to existing leader's warChart
        foreach(DiplomacyItem *d, diploItemList)
        {
            d->warChart.push_back(wh);
        }

        //Add existing leaders to new leader's warChart
        foreach(DiplomacyItem *d, diploItemList)
        {
            wh.nation = d->nation;
            wh.timesAtWarWith = 0;
            wh.warStartedOn = 0;
            wh.warStat = AT_PEACE;
            wh.hasMet = false;

            di->warChart.push_back(wh);
        }

        di->displayString = QString("Unmet Player %1 \t( ? )\n").arg(diploItemList.size());
        di->warChart.push_back(WarHistory{_nation, 0, 0, SELF});
    }
    else
    {
        di->displayString = QString("%1 (You)\t(%2)\n").arg(_name).arg(UnitController::NationName(_nation));
        di->warChart.push_back(WarHistory{_nation, 0, 0, SELF, true});
    }

    leaderListArea->addItem(di->displayString);
    diploItemList.push_back(di);
}

void Diplomacy::RemoveLeader(Nation nation)
{
    for(int i = 0; i < diploItemList.size(); i++)
    {
        if(diploItemList[i]->nation == nation)
        {
            leaderListArea->takeItem(i);
            diploItemList.remove(i);
            updatedDiploItems.push_back(UpdateStatus{i, UpdateFunc::REMOVE});
            break;
        }
    }
}

void Diplomacy::DeclareWarOn(Nation target, int targetIndex, Nation aggressor, int aggressorIndex)
{
    for(int i = 0; i < diploItemList.at(targetIndex)->warChart.size(); i++)
    {
        if(diploItemList.at(targetIndex)->warChart.at(i).nation == aggressor)
        {
            WarHistory wh{aggressor, turn, 0, AT_WAR};
            wh.timesAtWarWith = diploItemList.at(targetIndex)->warChart.at(i).timesAtWarWith + 1;

            if(diploItemList.at(targetIndex)->warChart.at(i).timesAtWarWith >= 4)
            {
                wh.warStat = PERMANENT_WAR;
            }

            diploItemList.at(targetIndex)->warChart.replace(i, wh);
            break;
        }
    }

    WarHistory wh{target, turn, 0, AT_WAR};
    wh.timesAtWarWith = diploItemList.at(aggressorIndex)->warChart.at(targetIndex).timesAtWarWith + 1;

    if(diploItemList.at(aggressorIndex)->warChart.at(targetIndex).timesAtWarWith >= 4)
    {
        wh.warStat = PERMANENT_WAR;
    }

    diploItemList.at(aggressorIndex)->warChart.replace(targetIndex, wh);

    updatedDiploItems.push_back(UpdateStatus{aggressorIndex, UpdateFunc::UDPATE});
    updatedDiploItems.push_back(UpdateStatus{targetIndex, UpdateFunc::UDPATE});

}

bool Diplomacy::MakePeaceWith(int resolverIndex, Nation resolverNation, int targetIndex, Nation targetNation)
{
    for(int i = 0; i < diploItemList.at(targetIndex)->warChart.size(); i++)
    {
        if(diploItemList.at(targetIndex)->warChart.at(i).nation == resolverNation)
        {
            WarHistory wh{resolverNation, 0, 0, AT_PEACE};
            wh.timesAtWarWith = diploItemList.at(targetIndex)->warChart.at(i).timesAtWarWith;
            wh.warStartedOn = diploItemList.at(targetIndex)->warChart.at(i).warStartedOn;
            diploItemList.at(targetIndex)->warChart.replace(i, wh);
            break;
        }
    }

    for(int i = 0; i < diploItemList.at(resolverIndex)->warChart.size(); i++)
    {
        if(diploItemList.at(resolverIndex)->warChart.at(i).nation == targetNation)
        {
            diploItemList.at(resolverIndex)->warChart.replace(i, WarHistory{resolverNation, diploItemList.at(resolverIndex)->warChart.at(i).warStartedOn, diploItemList.at(resolverIndex)->warChart.at(i).timesAtWarWith, AT_PEACE});
            return true;
        }
    }

    updatedDiploItems.push_back(UpdateStatus{resolverIndex, UpdateFunc::UDPATE});
    updatedDiploItems.push_back(UpdateStatus{targetIndex, UpdateFunc::UDPATE});

    return true;
}

void Diplomacy::selectLeader(QListWidgetItem *item)
{
    DiplomacyItem *d = diploItemList.at(leaderListArea->currentRow());
    if(d->warChart.at(localPlayerIndex).hasMet)
    {
        leaderName->setText(d->leaderName);
        leaderImage->setPixmap(d->image.scaled(350,350));
        leaderImage->setFixedSize(350, 350);
        nationName->setText(UnitController::NationName(d->nation));
    }
    else
    {
        leaderName->setText(QString("Unmet Player %1").arg(leaderListArea->currentRow()));
        leaderImage->setText("  ?  ");
        leaderImage->setFixedSize(350, 350);
        nationName->setText("Unknown");
    }

    QString war(" ");
    bool first = true;

    foreach(WarHistory wh, d->warChart)
    {
        if(wh.warStat == AT_WAR)
        {
            if(first)
            {
                war.append("At war with:\n\t");
                first = false;
            }
            war.append(UnitController::NationName(wh.nation));
            war.append("\n\t");
        }
    }
    atWarWith->setText(war);

    if((leaderListArea->currentRow() != localPlayerIndex) && d->warChart.at(localPlayerIndex).hasMet)
    {
        if(d->warChart.at(localPlayerIndex).warStat == AT_WAR || d->warChart.at(localPlayerIndex).warStat == PERMANENT_WAR)
        {
            declareWar->setEnabled(false);

            if(d->warChart.at(localPlayerIndex).warStat == AT_WAR)
                makePeace->setEnabled(true);
            else
                makePeace->setEnabled(false);
        }
        else
        {
            declareWar->setEnabled(true);
            makePeace->setEnabled(false);
        }
    }
    else
    {
        declareWar->setEnabled(false);
        makePeace->setEnabled(false);
    }
}

void Diplomacy::slot_delcareWar()
{
    Nation t = diploItemList.at(leaderListArea->currentRow())->nation;
    this->DeclareWarOn(t, leaderListArea->currentRow(), localPlayer, localPlayerIndex);
}

void Diplomacy::slot_makePeace()
{
    Nation t = diploItemList.at(leaderListArea->currentRow())->nation;
    this->MakePeaceWith(localPlayerIndex, localPlayer, leaderListArea->currentRow(), t);
}
