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
    sendingMessage = true;
//    QTcpSocket *socket = sender;
    QByteArray msg;
    msg.append(QString("%1__%2__%3__<=>").arg(msgData.type).arg(msgData.sender).arg(msgData.data));
    MessageQueue::instance()->put(msg);

    foreach(QTcpSocket *s, clients)
    {
        if(sender != NULL)
        {
            if((s == sender) && (msgData.type != PLAYER_SETUP_UPDATE && msgData.type != SYSTEM_MESSAGE))
               continue;
        }
        qDebug() << "[ConnectionManager]" << QString(msg);
        s->write(msg);        
        s->waitForBytesWritten();
    }

    sendingMessage = false;
}

void ConnectionManager::SendSingleMessage(MessageDataType msgData, QTcpSocket *receiver)
{
    sendingMessage = true;

    QByteArray msg;
    msg.append(QString("%1__%2__%3__<=>").arg(msgData.type).arg(msgData.sender).arg(msgData.data));
    qDebug() << "[ConnectionManager]" << msg;
    receiver->write(msg);

    sendingMessage = false;
}

void ConnectionManager::WaitForFinished()
{
    while(sendingMessage) {;}
}

int ConnectionManager::GetConnectionLocation(QTcpSocket *s)
{
    int i = 0;
    qDebug() << "[ConnectionManager]" << "clients size:" << clients.size();
    foreach(QTcpSocket *tcp, clients)
    {
        if(tcp == s)
        {
            qDebug() << "[ConnectionManager]" << "socket found at" << i;
            return i;
        }

        i++;
    }

    return 0;
}

QTcpSocket *ConnectionManager::GetSocket(int index)
{
    return clients.at(index);
}

