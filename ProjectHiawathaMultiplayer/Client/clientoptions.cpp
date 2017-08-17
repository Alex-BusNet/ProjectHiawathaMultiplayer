#include "clientoptions.h"
#include "clientmanager.h"
#include "Common/nation.h"
#include <QPainter>
#include <QWidget>
#include <QGridLayout>
#include <QPixmap>
#include <QDebug>
#include <QMediaPlayer>
#include <ui_clientoptions.h>
#include <Client/clienthandler.h>

typedef struct{QString CivName; QString LeaderName; QString PrimaryColor; QString SecondColor; int mapDimension1;
               int mapDimension2;}CivGameInfo;

ClientOptions::ClientOptions(QWidget *parent, QString username, bool fullscreen) :
    QMainWindow(parent),
    ui(new Ui::ClientOptions)
{
    ui->setupUi(this);
    FullScreen = fullscreen;
    user = username;
    game = NULL;
    this->setWindowFlags(Qt::FramelessWindowHint);

    QString OptionsStyle = "QWidget { background-color: #145e88; } QFrame { background-color: black; }";
    OptionsStyle += "QPushButton { background-color: #4899C8; border: 1px solid #f6b300; border-radius: 6px; font: 10px; min-width: 75px; max-width: 100px; min-height: 25px; max-height: 40px;}";
    OptionsStyle += "QPushButton:pressed { background-color: #77adcb; }";
    OptionsStyle += "QScrollBar:vertical { border: 2px sold black; background: #77adcb; width: 15px; margin: 12px 0 12px 0;} QScrollBar::handle:vertical { background: #4899C8; min-height: 10px; }";
    OptionsStyle += "QScrollBar::add-line:vertical { border: 1px solid black; background: #dedede; height: 10px; subcontrol-position: bottom; subcontrol-origin: margin; }  QScrollBar::sub-line:vertical { border: 1px solid black; height: 10px; background: #dedede; subcontrol-position: top; subcontrol-origin: margin; }";
    OptionsStyle += "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { border: 1px solid black; width: 3px; height: 3px; background: purple; } QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }";
    OptionsStyle += "QListView { background-color: white; } QListView::item { min-height: 50px; }";
    OptionsStyle += "QListView#civListWidget, #serverView { border: 5px inset #f6b300; show-decoration-selected: 1; } QLabel#playerLeaderName, #playerNationName { border: 2px inset #f6b300; background: #dedede; font: bold; max-height: 15px; min-width: 150px; }";
    OptionsStyle += "QComboBox { background-color: white; }";
    OptionsStyle += "QLineEdit { border: 2px inset #f6b300; background: #dedede; font: bold; max-height: 15px; min-width: 150px; }";
    OptionsStyle += "QTextBrowser { border: 2px inset #f6b300; background: white; font: normal; color: black; }";
    OptionsStyle += "QLineEdit#playerMessageBox { border: 1px inset #f6b300; background: white; font: normal; color: black; }" ;

    this->setStyleSheet(OptionsStyle);
    //Add the Civs to the list
    QListWidgetItem* item1 =new QListWidgetItem(tr("China"), ui->civListWidget);
    QListWidgetItem* item2 =new QListWidgetItem(tr("Germany"), ui->civListWidget);
    QListWidgetItem* item3 =new QListWidgetItem(tr("India"), ui->civListWidget);
    QListWidgetItem* item4 = new QListWidgetItem(tr("United States of America"),ui->civListWidget);
    QListWidgetItem* item5 =new QListWidgetItem(tr("Mongolia"), ui->civListWidget);
    QListWidgetItem* item6 =new QListWidgetItem(tr("Aztec Empire"), ui->civListWidget);
    QListWidgetItem* item7 =new QListWidgetItem(tr("France"), ui->civListWidget);
    QListWidgetItem* item8 = new QListWidgetItem(tr("Iroquois"), ui->civListWidget);
    QListWidgetItem* item9 = new QListWidgetItem(tr("Greece"), ui->civListWidget);
    QListWidgetItem* item10 = new QListWidgetItem(tr("Roman Empire"), ui->civListWidget);
    QListWidgetItem* item11 = new QListWidgetItem(tr("Russia"), ui->civListWidget);
    QListWidgetItem* item12 = new QListWidgetItem(tr("England"), ui->civListWidget);
    QListWidgetItem* item13 = new QListWidgetItem(tr("Egypt"), ui->civListWidget);
    QListWidgetItem* item14 = new QListWidgetItem(tr("Japan"), ui->civListWidget);
    QListWidgetItem* item15 = new QListWidgetItem(tr("Persia"), ui->civListWidget);
    QListWidgetItem* item16 = new QListWidgetItem(tr("Arabia"), ui->civListWidget);
    QListWidgetItem* item17 = new QListWidgetItem(tr("Random"), ui->civListWidget);


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

    item1->setIcon(icon1);
    item2->setIcon(icon2);
    item3->setIcon(icon3);
    item4->setIcon(icon4);
    item5->setIcon(icon5);
    item6->setIcon(icon6);
    item7->setIcon(icon7);
    item8->setIcon(icon8);
    item9->setIcon(icon9);
    item10->setIcon(icon10);
    item11->setIcon(icon11);
    item12->setIcon(icon12);
    item13->setIcon(icon13);
    item14->setIcon(icon14);
    item15->setIcon(icon15);
    item16->setIcon(icon16);
    item17->setIcon(icon17);

    ui->civListWidget->addItem(item1);
    ui->civListWidget->addItem(item2);
    ui->civListWidget->addItem(item3);
    ui->civListWidget->addItem(item4);
    ui->civListWidget->addItem(item5);
    ui->civListWidget->addItem(item6);
    ui->civListWidget->addItem(item7);
    ui->civListWidget->addItem(item8);
    ui->civListWidget->addItem(item9);
    ui->civListWidget->addItem(item10);
    ui->civListWidget->addItem(item11);
    ui->civListWidget->addItem(item12);
    ui->civListWidget->addItem(item13);
    ui->civListWidget->addItem(item14);
    ui->civListWidget->addItem(item15);
    ui->civListWidget->addItem(item16);
    ui->civListWidget->addItem(item17);

    ui->playerLeaderName->setText(username);
    ui->playerNationName->setText("Random");
    ui->playerLeaderImage->setPixmap(pic17);
    ui->playerMessageBox->setPlaceholderText("Say something...");
    ui->playerLeaderImage->setScaledContents(true);
    ui->readyPB->setEnabled(false);
    ClientHandler::instance()->SetClientOptionsObject(this);

}

void ClientOptions::UpdateServerInfo(MessageDataType msgData)
{
    ui->serverView->clear();

    QString info = msgData.data;
    QStringList sLst = info.split(';');
    QStringList sl;
    QString display;
    // 4 data format: sl[0] = Username, sl[1] = LeaderName, sl[2] = NationName, sl[3] = AI flag
    // 3 data format: sl[0] = Username, sl[1] = NationName, sl[2] = AI flag

    foreach(QString str, sLst)
    {
        if(str != "" && str != " ")
        {
            sl = str.split(',');
            if(sl.length() == 4)
            {
                if(sl[0] != "" && sl[0] != " ")
                    display = "[" + (sl[3] == "true" ? "AI" : sl[0]) + "] " + sl[1] + " (" + sl[2] + ")";
                else
                    display = "[" + (sl[3] == "true" ? QString("AI") : QString("Human")) + "] " + sl[1] + " (" + sl[2] + ")";

                if(GetNationEnum(sl[2]) != Random)
                    ui->civListWidget->item(GetNationEnum(sl[2]))->setFlags(ui->civListWidget->item(GetNationEnum(sl[2]))->flags() & ~Qt::ItemIsEnabled);
            }
            else if(sl.length() == 3)
            {
                display = "[" + (sl[2] == "true" ? QString("AI") : QString("Human")) + "] " + sl[0] + " (" + sl[1] + ")";

                if(GetNationEnum(sl[1]) != Random)
                    ui->civListWidget->item(GetNationEnum(sl[1]))->setFlags(ui->civListWidget->item(GetNationEnum(sl[1]))->flags() & ~Qt::ItemIsEnabled);
            }

            ui->serverView->addItem(new QListWidgetItem(display));
        }
    }
}

void ClientOptions::ReadChatMessage(MessageDataType msgData)
{
    ui->chatBox->append("[" + msgData.sender + "] " + msgData.data);
}

void ClientOptions::StartGame(int count)
{
    if(count == 10)
    {
        if(game != NULL)
            delete game;

        game = new ClientManager();
        this->close();
    }
    else
    {
        ui->readyPB->setText(QString("%1").arg(10 - count));
    }
}

ClientOptions::~ClientOptions()
{
    delete ui;
    delete game;
}

void ClientOptions::on_readyPB_clicked()
{
    /*
     * 1) Parse user info into a formatted string
     * 2) Send to server
     * 3) Await load game signal from server
     */

    MessageDataType msg;
    msg.type = PLAYER_SETUP_UPDATE;
    msg.sender = user;

    QString str;
    QListWidgetItem* selectedItem;
    if(ui->civListWidget->currentItem() == NULL)
    {
        selectedItem = ui->civListWidget->item(0);
    }
    else
    {
        selectedItem = ui->civListWidget->currentItem();
    }

    //Set the player as the correct country
    switch(ui->civListWidget->currentRow())
    {
    case 0:
        player = China;
        break;
    case 1:
        player = Germany;
        break;
    case 2:
        player = India;
        break;
    case 3:
        player = America;
        break;
    case 4:
        player = Mongolia;
        break;
    case 5:
        player = Aztec;
        break;
    case 6:
        player = France;
        break;
    case 7:
        player = Iroquois;
        break;
    case 8:
        player = Greece;
        break;
    case 9:
        player = Rome;
        break;
    case 10:
        player = Russia;
        break;
    case 11:
        player = England;
        break;
    case 12:
        player = Egypt;
        break;
    case 13:
        player = Japan;
        break;
    case 14:
        player = Persia;
        break;
    case 15:
        player = Arabia;
        break;
    default:
        player = Random;
        break;
    }

    str.append(user + "," + ui->playerLeaderName->text() + "," + ui->playerNationName->text() + ",false");
    msg.data = str;

    ClientHandler::instance()->sendMessage(msg);

    ui->civListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->civListWidget->setEnabled(false);
    ui->readyPB->setText("Waiting for\ngame start");
    ui->readyPB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    ui->readyPB->setEnabled(false);

    //Play a notification
//    QMediaPlayer *musicPlayer = new QMediaPlayer();
//    musicPlayer->setMedia(QUrl::fromLocalFile("Assets/Sound/notificationunitkilled.wav"));
//    musicPlayer->setVolume(50);
//    musicPlayer->play();

    //Start new game
//    game = new ClientManager(0, FullScreen, player, ui->hostIP->displayText(), ui->username->displayText());
//    this->close();

}

//This function updates the picture and text boxes to correpsond to currently selected civ
void ClientOptions::updateLeader()
{
    switch(ui->civListWidget->currentRow())
    {
    case 0:
        ui->playerLeaderName->setText("Mao Zedong");
        ui->playerNationName->setText("China");
        ui->playerLeaderImage->setPixmap(pic);
        break;
    case 1:
        ui->playerLeaderName->setText("Otto Von Bismarck");
        ui->playerNationName->setText("Germany");
        ui->playerLeaderImage->setPixmap(pic3);
        break;
    case 2:
        ui->playerLeaderName->setText("Gandhi");
        ui->playerNationName->setText("India");
        ui->playerLeaderImage->setPixmap(pic4);
        break;
    case 3:
        ui->playerLeaderName->setText("George Washington");
        ui->playerNationName->setText("United States of America");
        ui->playerLeaderImage->setPixmap(pic2);
        break;
    case 4:
        ui->playerLeaderName->setText("Genghis Khan");
        ui->playerNationName->setText("Mongolia");
        ui->playerLeaderImage->setPixmap(pic5);
        break;
    case 5:
        ui->playerLeaderName->setText("Montezuma");
        ui->playerNationName->setText("Aztecs");
        ui->playerLeaderImage->setPixmap(pic6);
        break;
    case 6:
        ui->playerLeaderName->setText("Napoleon Bonaparte");
        ui->playerNationName->setText("France");
        ui->playerLeaderImage->setPixmap(pic7);
        break;
    case 7:
        ui->playerLeaderName->setText("Hiawatha");
        ui->playerNationName->setText("Iroqois");
        ui->playerLeaderImage->setPixmap(pic8);
        break;
    case 8:
        ui->playerLeaderName->setText("Alexander");
        ui->playerNationName->setText("Greece");
        ui->playerLeaderImage->setPixmap(pic9);
        break;
    case 9:
        ui->playerLeaderName->setText("Julius Caesar");
        ui->playerNationName->setText("Rome");
        ui->playerLeaderImage->setPixmap(pic10);
        break;
    case 10:
        ui->playerLeaderName->setText("Josef Stalin");
        ui->playerNationName->setText("Russia");
        ui->playerLeaderImage->setPixmap(pic11);
        break;
    case 11:
        ui->playerLeaderName->setText("Elizabeth");
        ui->playerNationName->setText("England");
        ui->playerLeaderImage->setPixmap(pic12);
        break;
    case 12:
        ui->playerLeaderName->setText("Ramesses");
        ui->playerNationName->setText("Egypt");
        ui->playerLeaderImage->setPixmap(pic13);
        break;
    case 13:
        ui->playerLeaderName->setText("Oda Nobunaga");
        ui->playerNationName->setText("Japan");
        ui->playerLeaderImage->setPixmap(pic14);
        break;
    case 14:
        ui->playerLeaderName->setText("Cyrus");
        ui->playerNationName->setText("Persia");
        ui->playerLeaderImage->setPixmap(pic15);
        break;
    case 15:
        ui->playerLeaderName->setText("Harun al-Rashid");
        ui->playerNationName->setText("Arabia");
        ui->playerLeaderImage->setPixmap(pic16);
        break;
    default:
        ui->playerLeaderName->setText("Random");
        ui->playerNationName->setText("Random");
        ui->playerLeaderImage->setPixmap(pic17);
        break;
    }
}

//This calls the update functions when the item selecttion is changed
void ClientOptions::on_civListWidget_itemSelectionChanged()
{
    if((ui->civListWidget->currentItem()->flags() & Qt::ItemIsEnabled) == 32)
    {
        ui->readyPB->setEnabled(true);
        this->updateLeader();
        this->update();
    }
}

void ClientOptions::on_backPB_clicked()
{
    if(ClientHandler::instance()->disconnectFromHost())
        this->close();
}

void ClientOptions::on_playerMessageBox_returnPressed()
{
    ClientHandler::instance()->sendMessage(MessageDataType{CLIENT_TO_ALL, user , ui->playerMessageBox->text()});
    ui->chatBox->append("[" + user + "] " + ui->playerMessageBox->text() + "\n");
    ui->playerMessageBox->clear();

}
