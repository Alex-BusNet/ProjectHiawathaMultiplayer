/* The ServerOptions class allows for the
 * host to configure different aspects of the game
 * such as map size, number of civs, what civs are
 * Human controlled and what civs are AI controlled
 * as well as end game conditions. (Currently only maxTurns)
 *
 */

#include "serveroptions.h"
#include "servermanager.h"
#include <QWidget>
#include <ui_serveroptions.h>
#include <QDebug>
#include <Common/nation.h>

ServerOptions::ServerOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerOptions)
{
    ui->setupUi(this);
    server = NULL;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(740, 600);
    setup = true;
    ui->civList->setIconSize(QSize(64,64));

    /// Temporary, Will need to adjust the add/remove civ function
    /// to work well with map size changes.
    ui->addCivPB->setEnabled(false);
    ui->removeCivPB->setEnabled(false);

    QString OptionsStyle = "QWidget { background-color: #145e88; } QFrame { background-color: black; }";
    OptionsStyle += "QPushButton { background-color: #4899C8; border: 1px solid #f6b300; border-radius: 6px; font: 10px; min-width: 50px;}";
    OptionsStyle += "QPushButton:pressed { background-color: #77adcb; }";
    OptionsStyle += "QScrollBar:vertical { border: 2px sold black; background: #77adcb; width: 15px; margin: 12px 0 12px 0;} QScrollBar::handle:vertical { background: #4899C8; min-height: 10px; }";
    OptionsStyle += "QScrollBar::add-line:vertical { border: 1px solid black; background: #dedede; height: 10px; subcontrol-position: bottom; subcontrol-origin: margin; }  QScrollBar::sub-line:vertical { border: 1px solid black; height: 10px; background: #dedede; subcontrol-position: top; subcontrol-origin: margin; }";
    OptionsStyle += "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { border: 1px solid black; width: 3px; height: 3px; background: purple; } QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }";
    OptionsStyle += "QListView { background-color: white; } QListView::item { min-height: 80px; border-bottom-style: solid; border-bottom-color: black; border-bottom-width: 1px; }";
    OptionsStyle += "QListView { border: 2px inset #f6b300; show-decoration-selected: 1; } QListView::item:selected { color: black; }";
    OptionsStyle += "QLabel#leaderPortrait, #humanCountLabel, #aiCountLabel { border: 2px inset #f6b300; background: #dedede; max-height: 200px; max-width: 210px; }";
    OptionsStyle += "QComboBox { background-color: gray; border: 2px inset #f6b300; } QLabel#mapLabel { border: 2px inset #f6b300; background: #dedede; font: bold; max-height: 50px; max-width: 130px; } QLabel#civLabel { border: 2px inset #f6b300; background: #dedede; font: bold; max-height: 50px; max-width: 130px; }";
    OptionsStyle += "QLineEdit { border: 2px inset #f6b300; background: #dedede; font: bold; max-height: 15px; min-width: 150px; }";

    this->setStyleSheet(OptionsStyle);

    connect(MessageQueue::instance(), SIGNAL(MessageReady()), this, SLOT(ReadMessage()));

    //add the icon that correspond to the civ
    icon4 = QIcon("Assets/Leaders/Icons/USA.png");
    icon2 = QIcon("Assets/Leaders/Icons/GERMANY2.png");
    icon3 = QIcon("Assets/Leaders/Icons/INDIA.png");
    icon1 = QIcon("Assets/Leaders/Icons/CHINA.png");
    icon5 = QIcon("Assets/Leaders/Icons/MONGOLIA.jpg");
    icon6 = QIcon("Assets/Leaders/Icons/AZTEC.png");
    icon7 = QIcon("Assets/Leaders/Icons/FRANCE.png");
    icon8 = QIcon("Assets/Leaders/Icons/IROQUOIS.png");
    icon9 = QIcon("Assets/Leaders/Icons/GREECE.png");
    icon10 = QIcon("Assets/Leaders/Icons/ROME.png");
    icon11 = QIcon("Assets/Leaders/Icons/RUSSIA.png");
    icon12 = QIcon("Assets/Leaders/Icons/ENGLAND.png");
    icon13 = QIcon("Assets/Leaders/Icons/EGYPT.png");
    icon14 = QIcon("Assets/Leaders/Icons/JAPAN.png");
    icon15 = QIcon("Assets/Leaders/Icons/PERSIA.png");
    icon16 = QIcon("Assets/Leaders/Icons/ARABIA.png");
    icon17 = QIcon("Assets/Leaders/Icons/unknown.png");

    //Initialize the pics that correpsond to the civs
    pic2 = QPixmap("Assets/Leaders/George_head.jpg");
    pic3 = QPixmap("Assets/Leaders/bismark.jpg");
    pic4 = QPixmap("Assets/Leaders/gandhi.jpg");
    pic = QPixmap("Assets/Leaders/Mao.jpg");
    pic5 = QPixmap("Assets/Leaders/khan.jpg");
    pic6 = QPixmap("Assets/Leaders/montezuma.jpg");
    pic7 = QPixmap("Assets/Leaders/napoleon.jpg");
    pic8 = QPixmap("Assets/Leaders/Hiawatha.jpg");
    pic9 = QPixmap("Assets/Leaders/Alexander.jpg");
    pic10 = QPixmap("Assets/Leaders/Julius_Caesar.jpg");
    pic11 = QPixmap("Assets/Leaders/stalin.jpg");
    pic12 = QPixmap("Assets/Leaders/Queen_Elizabeth.jpg");
    pic13 = QPixmap("Assets/Leaders/Ramseses.jpg");
    pic14 = QPixmap("Assets/Leaders/Oda_Nobunga.jpg");
    pic15 = QPixmap("Assets/Leaders/Cyrus.jpg");
    pic16 = QPixmap("Assets/Leaders/Harun-Rashid.jpg");
    pic17 = QPixmap("Assets/Leaders/random.jpg");

    leaderImages << &pic2 << &pic3 << &pic4 << &pic << &pic5 << &pic6 << &pic7 << &pic8 << &pic9
                 << &pic10 << &pic11 << &pic12 << &pic13 << &pic14 << &pic15 << &pic16 << &pic17;

    nationIcons << &icon4 << &icon2 << &icon3 << &icon1 << &icon5 << &icon6 << &icon7 << &icon8 << &icon9
                << &icon10 << &icon11 << &icon12 << &icon13 << &icon14 << &icon15 << &icon16 << &icon17;

//    qDebug() << "adding Map options to mapSelect";
    ui->mapSelect->addItem("Duel: 40x24 - 2 Players",0);
    ui->mapSelect->addItem("Tiny: 56x36 - 4 Players",0);
    ui->mapSelect->addItem("Small: 66x42 - 6 Players",0);
    ui->mapSelect->addItem("Standard: 80x52 - 8 Players",0);
    ui->mapSelect->addItem("Large: 104x64 - 10 Players",0);
    ui->mapSelect->addItem("Huge: 128x80 - 12 Players",0);

//    qDebug() << "adding Civs to civSelect";
    ui->civSelect->addItem(icon4, "United States of America \n(George Washington)", 0);
    ui->civSelect->addItem(icon2, "Germany \n(Otto Von Bismark)", 0);
    ui->civSelect->addItem(icon3, "India \n(Gandhi)", 0);
    ui->civSelect->addItem(icon1, "China \n(Mao Zedong)", 0);
    ui->civSelect->addItem(icon5, "Mongolia \n(Genghis Khan)", 0);
    ui->civSelect->addItem(icon6, "Aztec \n(Montezuma)", 0);
    ui->civSelect->addItem(icon7, "France \n(Napoleon Bonaparte)", 0);
    ui->civSelect->addItem(icon8, "Iroquois \n(Hiawatha)", 0);
    ui->civSelect->addItem(icon9, "Greece \n(Alexander)", 0);
    ui->civSelect->addItem(icon10, "Rome \n(Julius Caesar)", 0);
    ui->civSelect->addItem(icon11, "Russia \n(Josef Stalin)", 0);
    ui->civSelect->addItem(icon12, "England \n(Elizabeth I)", 0);
    ui->civSelect->addItem(icon13, "Egypt \n(Ramesses)", 0);
    ui->civSelect->addItem(icon14, "Japan \n(Oda Nobunaga)", 0);
    ui->civSelect->addItem(icon15, "Persia \n(Cyrus)", 0);
    ui->civSelect->addItem(icon16, "Arabia \n(Harun al-Rashid)", 0);
    ui->civSelect->addItem(icon17, "Random", 0);

//    qDebug() <<"Setting leaderPortrait options";
    ui->leaderPortrait->setScaledContents(true);
    ui->leaderPortrait->setPixmap(pic17);
    ui->civSelect->setCurrentIndex(16);

    maxPlayers = 2;
    humanCount = 2;
    aiCount = 0;

    for(int i = 0; i < maxPlayers; i++)
    {
        CivInfo *ci = new CivInfo{"Random", Random, false, *nationIcons.at(16)};
        civs.push_back(ci);
        ui->civList->addItem(new QListWidgetItem(ci->civIcon, QString(ci->civName + "     [Human]")));        ui->civList->item(i)->setBackground(QBrush(Qt::white));
    }
    ui->aiRB->setEnabled(false);

    setup = false;
    ui->civList->setCurrentRow(0);
}

ServerOptions::~ServerOptions()
{
    ServerHandler::instance()->StopServer();
}

void ServerOptions::on_backToMenu_clicked()
{
    this->close();
}

void ServerOptions::on_startServer_clicked()
{
    //Parsing combo box strings to get the map size out of them
    QString str = ui->mapSelect->currentText();
    QString tempStr = str.remove(0,(str.indexOf(':',0)+1));
    QString tempStr2 = tempStr.remove((tempStr.indexOf('x',0)),20);
    int mapSize1 = tempStr2.toInt();
    mapSize1 = mapSize1 / 2;
    tempStr = str.remove(0,(str.indexOf('x',0)+1));
    tempStr2 = tempStr.remove((tempStr.indexOf('x',0)+3),20);
    int mapSize2 = tempStr2.toInt();

    QString info;
    foreach(CivInfo *ci, civs)
    {
        info.append(ci->civName + "," + NationName(ci->nation) + "," + (ci->isAi ? "true" : "false") + ";");
    }

    if(ServerHandler::instance()->StartServer(info, maxPlayers, mapSize1, mapSize2, aiCount))
    {
        ui->mapSelect->setEnabled(false);
        ui->aiRB->setEnabled(false);
        ui->humanRB->setEnabled(false);
        ui->civSelect->setEnabled(false);
    }

//    qDebug() << mapSize1 << mapSize2 << aiCount;
//    server = new ServerManager(0, mapSize1, mapSize2, aiCount);
//    server->show();
}

void ServerOptions::on_civSelect_currentIndexChanged(int index)
{
    if(!setup)
    {
        ui->leaderPortrait->setPixmap(*leaderImages.at(index));

        QString str = ui->civSelect->currentText();
        str.remove(0, str.indexOf('(', 0) + 1);
        str.remove(')');

        if(ui->aiRB->isChecked())
        {
            ui->civList->item(ui->civList->currentRow())->setText(QString(str + "     [AI]"));
        }
        else
        {
            ui->civList->item(ui->civList->currentRow())->setText(QString(str + "     [Human]"));
        }

        ui->civList->item(ui->civList->currentRow())->setIcon(ui->civSelect->itemIcon(index));

        civs.at(ui->civList->currentRow())->civName = ui->civSelect->currentText().remove(ui->civSelect->currentText().indexOf('(', 0), str.length()+1);
        civs.at(ui->civList->currentRow())->nation = static_cast<Nation>(ui->civSelect->currentIndex());
    }
}

void ServerOptions::on_mapSelect_currentIndexChanged(int index)
{
    if(!setup)
    {
        switch(index)
        {
        case 0:
            maxPlayers = 2;
            break;
        case 1:
            maxPlayers = 4;
            break;
        case 2:
            maxPlayers = 6;
            break;
        case 3:
            maxPlayers = 8;
            break;
        case 4:
            maxPlayers = 10;
            break;
        case 5:
            maxPlayers = 12;
            break;
        default:
            maxPlayers = 2;
            break;
        }

        if(maxPlayers > civs.size())
        {
//            qDebug() << "Adding more civs";
            for(int i = civs.size(); i < maxPlayers; i++)
            {
                CivInfo *ci = new CivInfo{"Random", Random, true, *nationIcons.at(16)};
                civs.push_back(ci);
                ui->civList->addItem(new QListWidgetItem(ci->civIcon, QString(ci->civName + "     [AI]")));
            }
        }
        else
        {
//            qDebug() << "Removing extra civs";
            for(int i = maxPlayers; i < civs.size(); i++)
            {
                civs.removeLast();
                ui->civList->takeItem(ui->civList->count() - 1);
            }
        }

        if(maxPlayers == 2)
            ui->aiRB->setEnabled(false);
        else
            ui->aiRB->setEnabled(true);
    }
}

void ServerOptions::updateCivs(bool updateCount)
{
//    qDebug() << "Updating civList";
    civs.at(ui->civList->currentRow())->civName = ui->civSelect->currentText();
    civs.at(ui->civList->currentRow())->nation = static_cast<Nation>(ui->civSelect->currentIndex());

    if(civs.at(ui->civList->currentRow())->isAi)
    {
        ui->aiRB->setChecked(true);
        ui->civList->item(ui->civList->currentRow())->setText(civs.at(ui->civList->currentRow())->civName + "     [AI]");
    }
    else
    {
        ui->humanRB->setChecked(true);
        ui->civList->item(ui->civList->currentRow())->setText(civs.at(ui->civList->currentRow())->civName + "     [Human]");
    }

    ui->civList->item(ui->civList->currentRow())->setIcon(ui->civSelect->itemIcon(ui->civSelect->currentIndex()));
}

void ServerOptions::on_civList_currentRowChanged(int currentRow)
{
    if(!setup)
    {
        ui->civSelect->setCurrentIndex(civs.at(currentRow)->nation);
        ui->leaderPortrait->setPixmap(*leaderImages.at(civs.at(currentRow)->nation));

        if(civs.at(currentRow)->isAi)
            ui->aiRB->setChecked(true);
        else
        {
            ui->humanRB->setChecked(true);
            ui->civSelect->setEnabled(false);
        }
    }
}

/*
 * Adds a new Civ to the list.
 * The civ will always default to
 * a player controlled civ.
 *
 */
void ServerOptions::on_addCivPB_clicked()
{
    CivInfo *ci = new CivInfo{"Random", Random, true, *nationIcons.at(16)};
    civs.push_back(ci);
    ui->civList->addItem(new QListWidgetItem(ci->civIcon, QString(ci->civName + "     [AI]")));
}

/*
 * Removes the last civ from the list
 */
void ServerOptions::on_removeCivPB_clicked()
{
    if(civs.size() > 2)
    {
        if(civs.last()->isAi)
        {
            aiCount--;
        }
        else
        {
            humanCount--;
        }

        civs.removeLast();
        ui->civList->takeItem(ui->civList->count() - 1);

    }
}

void ServerOptions::on_aiRB_clicked()
{
//    qDebug() << "AI Selected";
    ui->civList->item(ui->civList->currentRow())->setText(QString(civs.at(ui->civList->currentRow())->civName + "     [AI]"));
    civs.at(ui->civList->currentRow())->isAi = true;
    aiCount++;
    humanCount--;
}

void ServerOptions::on_humanRB_clicked()
{
//    qDebug() << "Human Selected";
    ui->civList->item(ui->civList->currentRow())->setText(QString(civs.at(ui->civList->currentRow())->civName + "     [Human]"));
    civs.at(ui->civList->currentRow())->isAi = false;
    humanCount++;
    aiCount--;
}

void ServerOptions::ReadMessage()
{
    QString str = MessageQueue::get();
    QStringList sl = str.split("__");
    if(sl.length() == 1)
        return;
    sl = sl[2].split(';');

    int i = 0;
    foreach(QString s, sl)
    {
        QStringList update = s.split(',');
        if(update.length() == 3)
        {
            civs.at(i)->civName = update[0];
            civs.at(i)->nation = GetNationEnum(update[1]);
            ui->civList->item(i)->setIcon(ui->civSelect->itemIcon(civs.at(i)->nation));
            ui->civList->item(i)->setText(civs.at(i)->civName + "     [" + (update[2] == "false" ? "Human" : "AI") + "]");
            i++;
        }
        else if(update.length() == 4)
        {
            civs.at(i)->civName = update[1];
            civs.at(i)->nation = GetNationEnum(update[2]);
            ui->civList->item(i)->setIcon(ui->civSelect->itemIcon(civs.at(i)->nation));
            ui->civList->item(i)->setText(update[1] + " (" + update[2] + ")     [" + (update[3] == "false" ? update[0] : "AI") + "]");
            i++;
        }
    }
}
