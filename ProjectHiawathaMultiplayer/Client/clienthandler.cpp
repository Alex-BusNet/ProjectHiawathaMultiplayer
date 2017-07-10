/* The ClientHandler class is responsible
 * for managing TCP (and UDP?) communications
 * between the client and server.
 *
 * This class sends the information of the
 * player's turn, as well as parse the server's
 * update of the AI and other players' turns.
 */

#include "clienthandler.h"

Q_GLOBAL_STATIC(ClientHandler, handler)

ClientHandler::ClientHandler()
{
}

ClientHandler *ClientHandler::instance()
{
    return handler;
}

void ClientHandler::SetHost(QString ip, int port)
{
    this->ip = QHostAddress(ip);
    this->port = port;
}

void ClientHandler::SetSocket(QTcpSocket *socket)
{
    this->socket = socket;

    connect(this->socket, SIGNAL(readyRead()), this, SLOT(ready()));
    connect(this->socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void ClientHandler::SetUser(QString name)
{
    username = name;
}

void ClientHandler::SetClientOptionsObject(ClientOptions *co)
{
    this->co = co;
}

void ClientHandler::sendMessage(MessageDataType msgData)
{
    /// TODO: Add Username and message type to message string
    if(socket == NULL)
        return;

    qDebug() << "[ClientHandler]" << "sendMessage()";
    socket->write(QString("%1__%2__%3").arg(msgData.type).arg(username).arg(msgData.data).toUtf8());
    socket->waitForBytesWritten();
}

bool ClientHandler::connectToHost()
{
    qDebug() << "[ClientHandler]" << "connectToHost()";
    if(port == 0 || ip.toString() == "0.0.0.0")
        return false;

    if(socket == NULL)
        return false;

    socket->connectToHost(ip, port);
    socket->waitForConnected(3000);

    if(socket->state() == QAbstractSocket::ConnectedState)
    {
//        sendMessage(MessageDataType{PLAYER_JOINED, username, "has joined"});
        return true;
    }
    return false;
}

bool ClientHandler::disconnectFromHost()
{
    QByteArray discon;
    discon.append("-1__SYS__" + username + "has disconnected");
    socket->write(discon);
    socket->waitForBytesWritten();
    socket->disconnectFromHost();

    if(socket->state() == QAbstractSocket::UnconnectedState)
        return true;
}

void ClientHandler::disconnected()
{
    qDebug() << "[ClientHandler]" << username << "disconnected()";
    QByteArray discon;
    discon.append("-1__SYS__" + username + "has disconnected");
    socket->write(discon);
    socket->waitForBytesWritten();
    socket->disconnectFromHost();

}

void ClientHandler::ready()
{
    QString str = QString(socket->readAll());
    qDebug() << "[ClientHandler]" << str;
    MessageDataType msg;
    QStringList sl = str.split("__");
    msg.type = sl[0].toInt();
    msg.sender = sl[1];
    msg.data = sl[2];

    if(co != NULL)
    {
        if(msg.type == PLAYER_SETUP_UPDATE)
            co->UpdateServerInfo(msg);
        else if((msg.type == CLIENT_TO_ALL) || (msg.type == SYSTEM_MESSAGE))
            co->ReadChatMessage(msg);
        else if(msg.type == GAME_START)
            co->StartGame();
    }
}
