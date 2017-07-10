#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "Server/connectionmanager.h"
#include "Server/messagequeue.h"
#include "servermanager.h"

class ServerManager;
class ServerHandler : public QTcpServer
{
    Q_OBJECT
public:
    inline ServerHandler() {}
    static ServerHandler* instance();

    bool StartServer(QString setupInfo, int maxPlayers, int mapX, int mapY, int numAI);
    void StopServer();

private:
    QStringList serverInfo;
    QString serverInfoStr;
    int playersReady, max;
    int x, y, ai;

    QTimer *countdown;
    ServerManager *sm;
    void SetMessageString();

protected:
    void incomingConnection(qintptr handle) override;

public slots:
    void SendMessage();
    void RemoveClient();

private slots:
    void StartGame();
};

#endif // SERVERHANDLER_H
