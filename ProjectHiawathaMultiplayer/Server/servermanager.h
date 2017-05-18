#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QWidget>

class ServerManager : public QWidget
{
    Q_OBJECT
public:
    explicit ServerManager(QWidget *parent = 0, int mapX = 40, int mapY = 24, int numAI = 0);
};

#endif // SERVERMANAGER_H
