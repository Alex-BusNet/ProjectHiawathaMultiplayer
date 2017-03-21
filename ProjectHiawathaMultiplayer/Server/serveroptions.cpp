#include "serveroptions.h"
#include "servermanager.h"
#include <QWidget>
#include <ui_serveroptions.h>

ServerOptions::ServerOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerOptions)
{
    ui->setupUi(this);
    server = NULL;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(520, 445);

    QString OptionsStyle = "QWidget { background-color: #145e88; } QFrame { background-color: black; }";
    OptionsStyle += "QPushButton { background-color: #4899C8; border: 1px solid #f6b300; border-radius: 6px; font: 10px; min-width: 100px;}";
    OptionsStyle += "QPushButton:pressed { background-color: #77adcb; }";
    OptionsStyle += "QScrollBar:vertical { border: 2px sold black; background: #77adcb; width: 15px; margin: 12px 0 12px 0;} QScrollBar::handle:vertical { background: #4899C8; min-height: 10px; }";
    OptionsStyle += "QScrollBar::add-line:vertical { border: 1px solid black; background: #dedede; height: 10px; subcontrol-position: bottom; subcontrol-origin: margin; }  QScrollBar::sub-line:vertical { border: 1px solid black; height: 10px; background: #dedede; subcontrol-position: top; subcontrol-origin: margin; }";
    OptionsStyle += "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { border: 1px solid black; width: 3px; height: 3px; background: purple; } QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }";
    OptionsStyle += "QListView { background-color: white; }";
    OptionsStyle += "QListView { border: 2px inset #f6b300; show-decoration-selected: 1; } QLabel#leaderPortrait { border: 2px inset #f6b300; background: #dedede; max-height: 200px; max-width: 210px; }";
    OptionsStyle += "QComboBox { background-color: gray; border: 2px inset #f6b300; } QLabel#mapLabel { border: 2px inset #f6b300; background: #dedede; font: bold; max-height: 50px; max-width: 130px; } QLabel#civLabel { border: 2px inset #f6b300; background: #dedede; font: bold; max-height: 50px; max-width: 130px; }";
    OptionsStyle += "QLineEdit { border: 2px inset #f6b300; background: #dedede; font: bold; max-height: 15px; min-width: 150px; }";

    this->setStyleSheet(OptionsStyle);

    //add the icon that correspond to the civ
    QIcon icon1("Assets/Leaders/Icons/CHINA.png");
    QIcon icon2("Assets/Leaders/Icons/GERMANY2.png");
    QIcon icon3("Assets/Leaders/Icons/INDIA.png");
    QIcon icon4("Assets/Leaders/Icons/USA.png");
    QIcon icon5("Assets/Leaders/Icons/MONGOLIA.jpg");
    QIcon icon6("Assets/Leaders/Icons/AZTEC.png");
    QIcon icon7("Assets/Leaders/Icons/FRANCE.png");
    QIcon icon8("Assets/Leaders/Icons/IROQUOIS.png");
    QIcon icon9("Assets/Leaders/Icons/GREECE.png");
    QIcon icon10("Assets/Leaders/Icons/ROME.png");
    QIcon icon11("Assets/Leaders/Icons/RUSSIA.png");
    QIcon icon12("Assets/Leaders/Icons/ENGLAND.png");
    QIcon icon13("Assets/Leaders/Icons/EGYPT.png");
    QIcon icon14("Assets/Leaders/Icons/JAPAN.png");
    QIcon icon15("Assets/Leaders/Icons/PERSIA.png");
    QIcon icon16("Assets/Leaders/Icons/ARABIA.png");
    QIcon icon17("Assets/Leaders/Icons/unknown.png");

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
    pic17 = QPixmap("Assets/Leaders/random.jpg");

    ui->mapSelect->addItem("Duel: 40x24 - 2 Players",0);
    ui->mapSelect->addItem("Tiny: 56x36 - 4 Players",0);
    ui->mapSelect->addItem("Small: 66x42 - 6 Players",0);
    ui->mapSelect->addItem("Standard: 80x52 - 8 Players",0);
    ui->mapSelect->addItem("Large: 104x64 - 10 Players",0);
    ui->mapSelect->addItem("Huge: 128x80 - 12 Players",0);

    ui->civSelect->addItem(icon17, "Random", 0);
    ui->civSelect->addItem(icon1, "China (Mao Zedong)", 0);
    ui->civSelect->addItem(icon2, "Germany (Otto Von Bismark)", 0);
    ui->civSelect->addItem(icon4, "India (Gandhi)", 0);
    ui->civSelect->addItem(icon3, "United States of America (George Washington)", 0);
    ui->civSelect->addItem(icon5, "Mongolia (Genghis Khan)", 0);
    ui->civSelect->addItem(icon6, "Aztec (Montezuma)", 0);
    ui->civSelect->addItem(icon7, "France (Napoleon Bonaparte)", 0);
    ui->civSelect->addItem(icon8, "Iroquois (Hiawatha)", 0);
    ui->civSelect->addItem(icon9, "Greece (Alexander)", 0);
    ui->civSelect->addItem(icon10, "Rome (Julius Caesar)", 0);
    ui->civSelect->addItem(icon11, "Russia (Josef Stalin)", 0);
    ui->civSelect->addItem(icon12, "England (Elizabeth I)", 0);
    ui->civSelect->addItem(icon13, "Egypt (Ramesses)", 0);
    ui->civSelect->addItem(icon14, "Japan (Oda Nobunaga)", 0);
    ui->civSelect->addItem(icon15, "Persia (Cyrus)", 0);
    ui->civSelect->addItem(icon16, "Arabia (Harun al-Rashid)", 0);

    ui->leaderPortrait->setScaledContents(true);
    ui->leaderPortrait->setPixmap(pic17);
}

ServerOptions::~ServerOptions()
{

}

void ServerOptions::on_backToMenu_clicked()
{
    this->close();
}

void ServerOptions::on_startServer_clicked()
{
    int index = ui->mapSelect->currentIndex();
    int numOfAI;
    switch(index)
    {
    case 0:
        numOfAI = 1;
        break;
    case 1:
        numOfAI = 3;
        break;
    case 2:
        numOfAI = 5;
        break;
    case 3:
        numOfAI = 7;
        break;
    case 4:
        numOfAI = 9;
        break;
    case 5:
        numOfAI = 11;
        break;
    default:
        numOfAI = 3;

    }

    //Parsing combo box strings to get the map size out of them
    QString str = ui->mapSelect->currentText();
    QString tempStr = str.remove(0,(str.indexOf(':',0)+1));
    QString tempStr2 = tempStr.remove((tempStr.indexOf('x',0)),20);
    int mapSize1 = tempStr2.toInt();
    mapSize1 = mapSize1 / 2;
    tempStr = str.remove(0,(str.indexOf('x',0)+1));
    tempStr2 = tempStr.remove((tempStr.indexOf('x',0)+3),20);
    int mapSize2 = tempStr2.toInt();
}

void ServerOptions::on_civSelect_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        ui->leaderPortrait->setPixmap(pic17);
        break;
    case 1:
        ui->leaderPortrait->setPixmap(pic);
        break;
    case 2:
        ui->leaderPortrait->setPixmap(pic3);
        break;
    case 3:
        ui->leaderPortrait->setPixmap(pic4);
        break;
    case 4:
        ui->leaderPortrait->setPixmap(pic2);
        break;
    case 5:
        ui->leaderPortrait->setPixmap(pic5);
        break;
    case 6:
        ui->leaderPortrait->setPixmap(pic6);
        break;
    case 7:
        ui->leaderPortrait->setPixmap(pic7);
        break;
    case 8:
        ui->leaderPortrait->setPixmap(pic8);
        break;
    case 9:
        ui->leaderPortrait->setPixmap(pic9);
        break;
    case 10:
        ui->leaderPortrait->setPixmap(pic10);
        break;
    case 11:
        ui->leaderPortrait->setPixmap(pic11);
        break;
    case 12:
        ui->leaderPortrait->setPixmap(pic12);
        break;
    case 13:
        ui->leaderPortrait->setPixmap(pic13);
        break;
    case 14:
        ui->leaderPortrait->setPixmap(pic14);
        break;
    case 15:
        ui->leaderPortrait->setPixmap(pic15);
        break;
    case 16:
        ui->leaderPortrait->setPixmap(pic16);
        break;
    default:
        ui->leaderPortrait->setPixmap(pic17);
        break;
    }
}
