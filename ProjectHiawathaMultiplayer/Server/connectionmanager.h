#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QTcpSocket>
#include <Server/messagequeue.h>
#include <Common/datatypes.h>
#include <QHostAddress>

class ConnectionManager
{
public:
    inline ConnectionManager() {}
    static ConnectionManager *instance();

    void AddClient(QTcpSocket *socket);
    void RemoveClient();
    void RemoveClient(QTcpSocket *socket);
    void SetMaxConnections(int max);

    bool CanAddConnection();
    void BroadcastMessage(MessageDataType msgData, QTcpSocket *sender);
    void SendSingleMessage(MessageDataType msgData, QTcpSocket *receiver);

    int GetConnectionLocation(QTcpSocket *s);
    QTcpSocket* GetSocket(int index);
private:
    int maxConnections;
    QList<QTcpSocket*> clients;
};

#endif // CONNECTIONMANAGER_H
