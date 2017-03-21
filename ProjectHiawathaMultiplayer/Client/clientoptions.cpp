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

typedef struct{QString CivName; QString LeaderName; QString PrimaryColor; QString SecondColor; int mapDimension1;
               int mapDimension2;}CivInfo;

ClientOptions::ClientOptions(QWidget *parent, bool fullscreen) :
    QWidget(parent),
    ui(new Ui::ClientOptions)
{
    ui->setupUi(this);
    FullScreen = fullscreen;
    game = NULL;
    this->setWindowFlags(Qt::FramelessWindowHint);

    QString OptionsStyle = "QWidget { background-color: #145e88; } QFrame { background-color: black; }";
    OptionsStyle += "QPushButton { background-color: #4899C8; border: 1px solid #f6b300; border-radius: 6px; font: 10px; min-width: 100px;}";
    OptionsStyle += "QPushButton:pressed { background-color: #77adcb; }";
    OptionsStyle += "QScrollBar:vertical { border: 2px sold black; background: #77adcb; width: 15px; margin: 12px 0 12px 0;} QScrollBar::handle:vertical { background: #4899C8; min-height: 10px; }";
    OptionsStyle += "QScrollBar::add-line:vertical { border: 1px solid black; background: #dedede; height: 10px; subcontrol-position: bottom; subcontrol-origin: margin; }  QScrollBar::sub-line:vertical { border: 1px solid black; height: 10px; background: #dedede; subcontrol-position: top; subcontrol-origin: margin; }";
    OptionsStyle += "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { border: 1px solid black; width: 3px; height: 3px; background: purple; } QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }";
    OptionsStyle += "QListView { background-color: white; }";
    OptionsStyle += "QListView#listWidget { border: 5px inset #f6b300; show-decoration-selected: 1; } QLabel#label_3, #label_2, #label_4, #label_5 { border: 2px inset #f6b300; background: #dedede; font: bold; max-height: 15px; min-width: 150px; }";
    OptionsStyle += "QComboBox { background-color: white; }";
    OptionsStyle += "QLineEdit { border: 2px inset #f6b300; background: #dedede; font: bold; max-height: 15px; min-width: 150px; }";

    this->setStyleSheet(OptionsStyle);
    //Add the Civs to the list
    QListWidgetItem* item1 =new QListWidgetItem(tr("China"), ui->listWidget);
    QListWidgetItem* item2 =new QListWidgetItem(tr("Germany"), ui->listWidget);
    QListWidgetItem* item3 =new QListWidgetItem(tr("India"), ui->listWidget);
    QListWidgetItem* item4 = new QListWidgetItem(tr("United States of America"),ui->listWidget);
    QListWidgetItem* item5 =new QListWidgetItem(tr("Mongolia"), ui->listWidget);
    QListWidgetItem* item6 =new QListWidgetItem(tr("Aztec Empire"), ui->listWidget);
    QListWidgetItem* item7 =new QListWidgetItem(tr("France"), ui->listWidget);
    QListWidgetItem* item8 = new QListWidgetItem(tr("Iroquois"), ui->listWidget);
    QListWidgetItem* item9 = new QListWidgetItem(tr("Greece"), ui->listWidget);
    QListWidgetItem* item10 = new QListWidgetItem(tr("Roman Empire"), ui->listWidget);
    QListWidgetItem* item11 = new QListWidgetItem(tr("Russia"), ui->listWidget);
    QListWidgetItem* item12 = new QListWidgetItem(tr("England"), ui->listWidget);
    QListWidgetItem* item13 = new QListWidgetItem(tr("Egypt"), ui->listWidget);
    QListWidgetItem* item14 = new QListWidgetItem(tr("Japan"), ui->listWidget);
    QListWidgetItem* item15 = new QListWidgetItem(tr("Persia"), ui->listWidget);
    QListWidgetItem* item16 = new QListWidgetItem(tr("Arabia"), ui->listWidget);


    QIcon* icon1 = new QIcon();
    QIcon* icon2 = new QIcon();
    QIcon* icon3 = new QIcon();
    QIcon* icon4 = new QIcon();
    QIcon* icon5 = new QIcon();
    QIcon* icon6 = new QIcon();
    QIcon* icon7 = new QIcon();
    QIcon* icon8 = new QIcon();
    QIcon* icon9 = new QIcon();
    QIcon* icon10 = new QIcon();
    QIcon* icon11 = new QIcon();
    QIcon* icon12 = new QIcon();
    QIcon* icon13 = new QIcon();
    QIcon* icon14 = new QIcon();
    QIcon* icon15 = new QIcon();
    QIcon* icon16 = new QIcon();
    //add the icon that correspond to the civ
    icon1->addFile("Assets/Leaders/Icons/CHINA.png");
    icon2->addFile("Assets/Leaders/Icons/GERMANY2.png");
    icon3->addFile("Assets/Leaders/Icons/INDIA.png");
    icon4->addFile("Assets/Leaders/Icons/USA.png");
    icon5->addFile("Assets/Leaders/Icons/MONGOLIA.jpg");
    icon6->addFile("Assets/Leaders/Icons/AZTEC.png");
    icon7->addFile("Assets/Leaders/Icons/FRANCE.png");
    icon8->addFile("Assets/Leaders/Icons/IROQUOIS.png");
    icon9->addFile("Assets/Leaders/Icons/GREECE.png");
    icon10->addFile("Assets/Leaders/Icons/ROME.png");
    icon11->addFile("Assets/Leaders/Icons/RUSSIA.png");
    icon12->addFile("Assets/Leaders/Icons/ENGLAND.png");
    icon13->addFile("Assets/Leaders/Icons/EGYPT.png");
    icon14->addFile("Assets/Leaders/Icons/JAPAN.png");
    icon15->addFile("Assets/Leaders/Icons/PERSIA.png");
    icon16->addFile("Assets/Leaders/Icons/ARABIA.png");

    item1->setIcon(*icon1);
    item2->setIcon(*icon2);
    item3->setIcon(*icon3);
    item4->setIcon(*icon4);
    item5->setIcon(*icon5);
    item6->setIcon(*icon6);
    item7->setIcon(*icon7);
    item8->setIcon(*icon8);
    item9->setIcon(*icon9);
    item10->setIcon(*icon10);
    item11->setIcon(*icon11);
    item12->setIcon(*icon12);
    item13->setIcon(*icon13);
    item14->setIcon(*icon14);
    item15->setIcon(*icon15);
    item16->setIcon(*icon16);

    ui->listWidget->addItem(item1);
    ui->listWidget->addItem(item2);
    ui->listWidget->addItem(item3);
    ui->listWidget->addItem(item4);
    ui->listWidget->addItem(item5);
    ui->listWidget->addItem(item6);
    ui->listWidget->addItem(item7);
    ui->listWidget->addItem(item8);
    ui->listWidget->addItem(item9);
    ui->listWidget->addItem(item10);
    ui->listWidget->addItem(item11);
    ui->listWidget->addItem(item12);
    ui->listWidget->addItem(item13);
    ui->listWidget->addItem(item14);
    ui->listWidget->addItem(item15);
    ui->listWidget->addItem(item16);


    //Initialize the pics that correpsond to the civs
    pic = QPixmap("Assets/Leaders/Mao.jpg");
    pic2 = QPixmap("Assets/Leaders/George_head.jpg");
    pic3 = QPixmap("Assets/Leaders/bismark.jpg");
    pic4 = QPixmap("Assets/Leaders/gandhi.jpg");
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

    ui->label_3->setText("Gandhi");
    ui->label->setPixmap(pic4);

    ui->label->setScaledContents(true);
    ui->pushButton->setFlat(true);

    ui->hostIP->setText("10.74.17.34");

}

ClientOptions::~ClientOptions()
{
    delete ui;
    delete game;
}

void ClientOptions::on_pushButton_clicked()
{
    QListWidgetItem* selectedItem;
    if(ui->listWidget->currentItem() == NULL)
    {
        selectedItem = ui->listWidget->item(0);
    }else
    {
        selectedItem = ui->listWidget->currentItem();
    }


//    CivInfo info = {selectedItem->text(),ui->label_3->text(),"red","blue",mapSize1,mapSize2};//This is data that needs passed
    if(game != NULL)
    {
        delete game;
    }
    //Set the player as the correct country
    switch(ui->listWidget->currentRow())
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
        player = India;
        break;
    }
    //Play a notification
    QMediaPlayer *musicPlayer = new QMediaPlayer();
    musicPlayer->setMedia(QUrl::fromLocalFile("Assets/Sound/notificationunitkilled.wav"));
    musicPlayer->setVolume(50);
    musicPlayer->play();
    //Start new game
    game = new ClientManager(0, FullScreen, player, ui->hostIP->displayText(), ui->username->displayText());
    this->close();

}

//This function updates the picture and text boxes to correpsond to currently selected civ
void ClientOptions::updateLeader()
{
    switch(ui->listWidget->currentRow())
    {
    case 0:
        ui->label_3->setText("Mao Zedong");
        ui->label->setPixmap(pic);
        break;
    case 1:
        ui->label_3->setText("Otto Von Bismarck");
        ui->label->setPixmap(pic3);
        break;
    case 2:
        ui->label_3->setText("Gandhi");
        ui->label->setPixmap(pic4);
        break;
    case 3:
        ui->label_3->setText("George Washington");
        ui->label->setPixmap(pic2);
        break;
    case 4:
        ui->label_3->setText("Genghis Khan");
        ui->label->setPixmap(pic5);
        break;
    case 5:
        ui->label_3->setText("Montezuma");
        ui->label->setPixmap(pic6);
        break;
    case 6:
        ui->label_3->setText("Napoleon Bonaparte");
        ui->label->setPixmap(pic7);
        break;
    case 7:
        ui->label_3->setText("Hiawatha");
        ui->label->setPixmap(pic8);
        break;
    case 8:
        ui->label_3->setText("Alexander");
        ui->label->setPixmap(pic9);
        break;
    case 9:
        ui->label_3->setText("Julius Caesar");
        ui->label->setPixmap(pic10);
        break;
    case 10:
        ui->label_3->setText("Josef Stalin");
        ui->label->setPixmap(pic11);
        break;
    case 11:
        ui->label_3->setText("Elizabeth");
        ui->label->setPixmap(pic12);
        break;
    case 12:
        ui->label_3->setText("Ramesses");
        ui->label->setPixmap(pic13);
        break;
    case 13:
        ui->label_3->setText("Oda Nobunaga");
        ui->label->setPixmap(pic14);
        break;
    case 14:
        ui->label_3->setText("Cyrus");
        ui->label->setPixmap(pic15);
        break;
    case 15:
        ui->label_3->setText("Harun al-Rashid");
        ui->label->setPixmap(pic16);
        break;
    default:
        ui->label_3->setText("Gandhi");
        ui->label->setPixmap(pic4);
        break;
    }
}

//This calls the update functions when the item selecttion is changed
void ClientOptions::on_listWidget_itemSelectionChanged()
{
    ui->pushButton->setFlat(false);
    this->updateLeader();
    this->update();
}

void ClientOptions::on_pushButton_2_clicked()
{
    this->close();
}
