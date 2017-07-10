#include "clientmanager.h"

#include <QMessageBox>


ClientManager::ClientManager(QWidget *parent, bool fullscreen, Nation player, QString IP, QString username) :
    QWidget(parent)
{
    this->setWindowTitle("Project Hiawatha Client");
    QMessageBox *mbox = new QMessageBox();
    mbox->setText("Waiting for map info from server");
    mbox->show();
}

ClientManager::~ClientManager()
{

}
