#ifndef CLIENTOPTIONS_H
#define CLIENTOPTIONS_H

#include <QWidget>
#include <QListWidgetItem>
#include "Client/clientmanager.h"
#include "Common/nation.h"
#include <Common/datatypes.h>

namespace Ui {
class ClientOptions;
}

class ClientOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ClientOptions(QWidget *parent = 0, QString username = "New Player", bool fullscreen = false);

    void UpdateServerInfo(MessageDataType msgData);
    void ReadChatMessage(MessageDataType msgData);
    void StartGame();
    ~ClientOptions();

private slots:
    void on_readyPB_clicked();

    void updateLeader();

    void on_civListWidget_itemSelectionChanged();

    void on_backPB_clicked();

    void on_playerMessageBox_returnPressed();

private:
    Ui::ClientOptions *ui;
    ClientManager *game;
    bool FullScreen;
    Nation player;
    QString user;
    QPixmap pic;    //("../ProjectHiawatha/Assets/Leaders/Mao.jpg");
    QPixmap pic2;   //("../ProjectHiawatha/Assets/Leaders/George_head.jpg");
    QPixmap pic3;   //("../ProjectHiawatha/Assets/Leaders/bismark.jpg");
    QPixmap pic4;   //("../ProjectHiawatha/Assets/Leaders/gandhi.jpg");
    QPixmap pic5;   //("../ProjectHiawatha/Assets/Leaders/khan.jpg");
    QPixmap pic6;   //("../ProjectHiawatha/Assets/Leaders/montezuma.jpg");
    QPixmap pic7;   //("../ProjectHiawatha/Assets/Leaders/napoleon.jpg");
    QPixmap pic8;   //("../ProjectHiawatha/Assets/Leaders/Hiawatha.jpg");
    QPixmap pic9;   //("../ProjectHiawatha/Assets/Leaders/Alexander.jpg");
    QPixmap pic10;  //("../ProjectHiawatha/Assets/Leaders/Julius_Caesar.jpg");
    QPixmap pic11;  //("../ProjectHiawatha/Assets/Leaders/stalin.jpg");
    QPixmap pic12;  //("../ProjectHiawatha/Assets/Leaders/Queen_Elizabeth.jpg");
    QPixmap pic13;  //("../ProjectHiawatha/Assets/Leaders/Ramseses.jpg");
    QPixmap pic14;  //("../ProjectHiawatha/Assets/Leaders/Oda_Nobunga.jpg");
    QPixmap pic15;  //("../ProjectHiawatha/Assets/Leaders/Cyrus.jpg");
    QPixmap pic16;  //("../ProjectHiawatha/Assets/Leaders/Harun-Rashid.jpg");
    QPixmap pic17;  //("../ProjectHiawatha/Assets/Leaders/random.jpg")

    //add the icon that correspond to the civ
    QIcon icon4;
    QIcon icon2;
    QIcon icon3;
    QIcon icon1;
    QIcon icon5;
    QIcon icon6;
    QIcon icon7;
    QIcon icon8;
    QIcon icon9;
    QIcon icon10;
    QIcon icon11;
    QIcon icon12;
    QIcon icon13;
    QIcon icon14;
    QIcon icon15;
    QIcon icon16;
    QIcon icon17;
};

#endif // CLIENTOPTIONS_H
