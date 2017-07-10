#include "connectionmanager.h"

Q_GLOBAL_STATIC(ConnectionManager, connectionManager)

ConnectionManager *ConnectionManager::instance()
{
    return connectionManager;
}

void ConnectionManager::AddClient(QTcpSocket *socket)
{
    clients << socket;
}

void ConnectionManager::RemoveClient()
{
    foreach(QTcpSocket* tcp, clients)
    {
        tcp->disconnectFromHost();
        tcp->deleteLater();
    }
}

void ConnectionManager::RemoveClient(QTcpSocket *socket)
{
    qDebug() << "[ConnectionManager] RemoveClient(QTcpSocket*)";
    int indexOfSocket = clients.indexOf(socket);
    clients.removeAll(socket);
    MessageQueue::instance()->put("-remove-;" + QString::number(indexOfSocket) + ";" + QString::number(socket->socketDescriptor()));
}

void ConnectionManager::SetMaxConnections(int max)
{
    maxConnections = max;
}

bool ConnectionManager::CanAddConnection()
{
    if(maxConnections > clients.size())
    {
        return true;
    }

    return false;
}

void ConnectionManager::BroadcastMessage(MessageDataType msgData, QTcpSocket *sender)
{
    qDebug() << "[ConnectionManager] BroadcastMessage()";

    QTcpSocket *socket = sender;
    QByteArray msg;
    msg.append(QString("%1__%2__%3").arg(msgData.type).arg(msgData.sender).arg(msgData.data));
    MessageQueue::instance()->put(msg);

    foreach(QTcpSocket *s, clients)
    {
        if((s == socket) && (msgData.type != PLAYER_SETUP_UPDATE && msgData.type != SYSTEM_MESSAGE))
            continue;

        s->write(msg);
    }
}

void ConnectionManager::SendSingleMessage(MessageDataType msgData, QTcpSocket *receiver)
{
    QByteArray msg;
    qDebug() << "[ConnectionManager]" << msgData.sender << msgData.data;
    msg.append(QString("%1__%2__%3").arg(msgData.type).arg(msgData.sender).arg(msgData.data));
    receiver->write(msg);
}

int ConnectionManager::GetConnectionLocation(QTcpSocket *s)
{
    int i = 0;
    foreach(QTcpSocket *tcp, clients)
    {
        if(tcp == s)
            return i;

        i++;
    }
}

