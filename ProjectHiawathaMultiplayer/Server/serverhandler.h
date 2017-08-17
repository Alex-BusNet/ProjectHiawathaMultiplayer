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

    bool StartServer(QString IP, int maxPlayers, int mapX, int mapY, int numAI, QVector<CivInfo *> ci);
    void StopServer();
    void SendClientUpdate(MessageTypes type, QString msg);

private:
    QStringList serverInfo;
    QString serverInfoStr;
    int playersReady, max;
    int x, y, ai;
//    QVector<int> playerSelections;
//    QVector<bool> aiStatus;
    QVector<CivInfo*> civInfo;
    QTimer *countdown;
    ServerManager *sm;
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
