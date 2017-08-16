/* The ServerHandler class is responsible for
 * managing all communication and connections between
 * the multiple clients so that everyone has the same information.
 *
 */

#include "serverhandler.h"
#include <QDebug>

Q_GLOBAL_STATIC(ServerHandler, serverHandler)

ServerHandler *ServerHandler::instance()
{
    return serverHandler;
}

bool ServerHandler::StartServer(QString IP, QString setupInfo, int maxPlayers, int mapX, int mapY, int numAI)
{
    if(this->listen(QHostAddress(IP), 5300))
    {
//        MessageQueue::instance()->put("-1,SYS,Server Started. Listening on: " + this->serverAddress().toString() + ":5300");
        serverInfo = setupInfo.split(';');
        max = maxPlayers;
        x = mapX;
        y = mapY;
        ai = numAI;
        playersReady = 0;
        qDebug() << "[ServerHandler]" << serverInfo;
        SetMessageString();
        return true;

        for(int i = 0 ; i < max; i++)
        {
            playerSelections.push_back((int)Random);
            aiStatus.push_back(false);
        }

        for(int i = 0; i < ai; i++)
        {
            playerSelections.push_back((int)Random);
            aiStatus.push_back(true);
        }
    }
    else
    {
        MessageQueue::instance()->put("SYS;Setup error");
        qDebug() << "[ServerHandler]" << "Server setup error";
        return false;
    }
}

void ServerHandler::StopServer()
{
    qDebug() << "[ServerHandler]" << "stopServer()";
    ConnectionManager::instance()->RemoveClient();
    this->deleteLater();
}

void ServerHandler::SendClientUpdate(MessageTypes type, QString msg)
{

    if(type == TURN_END || type == TURN_START)
    {
        ConnectionManager::instance()->SendSingleMessage(MessageDataType{type, "SYS", "StartTurn"}, ConnectionManager::instance()->GetSocket(msg.toInt()));
    }
    else
    {
        ConnectionManager::instance()->BroadcastMessage(MessageDataType{type, "SYS", msg}, NULL);
    }
}

void ServerHandler::SetMessageString()
{
    serverInfoStr = "";
    foreach(QString str, serverInfo)
    {
        if(str != "" && str != " ")
            serverInfoStr += str + ";";
    }

    qDebug() << "[ServerHandler]" << serverInfoStr;
}

void ServerHandler::incomingConnection(qintptr handle)
{
    qDebug() << "[ServerHandler]" << "incomingConnection()";
    QTcpSocket *socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);
    connect(socket, SIGNAL(readyRead()), ServerHandler::instance(), SLOT(SendMessage()));
    connect(socket, SIGNAL(disconnected()), ServerHandler::instance(), SLOT(RemoveClient()));
    ConnectionManager::instance()->AddClient(socket);
    MessageDataType msg{PLAYER_SETUP_UPDATE, "SYS", serverInfoStr};
    ConnectionManager::instance()->SendSingleMessage(msg, socket);
}

void ServerHandler::SendMessage()
{
    qDebug() << "[ServerHandler]" << "sendMessage()";
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    QString str(s->readAll());
    QStringList sl = str.split("__");
    qDebug() << "[ServerHandler]" << sl;

    if(sl[0].toInt() == CLIENT_TO_ALL)
    {
        ConnectionManager::instance()->BroadcastMessage(MessageDataType{CLIENT_TO_ALL, sl[1], sl[2]}, s);
    }
    else if(sl[0].toInt() == PLAYER_SETUP_UPDATE)
    {
        int updated = ConnectionManager::instance()->GetConnectionLocation(s);
        playersReady++;
        serverInfo[updated] = sl[2];
//        playerSelections.replace(updated, (int)GetNationEnum(sl[2].split(",")[2]));
        SetMessageString();
        ConnectionManager::instance()->BroadcastMessage(MessageDataType{PLAYER_SETUP_UPDATE, "SYS", serverInfoStr}, s);
        MessageQueue::put(serverInfoStr);

        if(playersReady == 1)
        {
            // Start countdown timer;
            countdown = new QTimer();
            countdown->setInterval(1000);
            connect(countdown, SIGNAL(timeout()), this, SLOT(StartGame()));
            countdown->start();
            // Send messages to clients.
        }
    }
    else if(sl[0].toInt() == PLAYER_JOINED)
    {
        ConnectionManager::instance()->BroadcastMessage(MessageDataType{PLAYER_JOINED, "SYS", sl[1] + " " + sl[3]}, s);
    }
}

void ServerHandler::RemoveClient()
{
    qDebug() << "[ServerHandler]" << "removeClient()";
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    ConnectionManager::instance()->RemoveClient(s);
    playersReady--;
}

void ServerHandler::StartGame()
{
    static int i = 0;

    if(i == 9)
        ConnectionManager::instance()->BroadcastMessage(MessageDataType{GAME_START, "SYS", QString("%1").arg(i)}, NULL);
    else
        ConnectionManager::instance()->BroadcastMessage(MessageDataType{SYSTEM_MESSAGE, "SYS", QString("%1").arg(i)}, NULL);

    if(i == 9)
    {
        if(sm != NULL)
            delete sm;

        sm = new ServerManager(NULL, x, y, playerSelections, aiStatus);
        qDebug() << "[ServerHandler]" << "ServerManager Init complete";
        sm->show();

        countdown->stop();
    }
    else
    {        
        i++;
    }
}
