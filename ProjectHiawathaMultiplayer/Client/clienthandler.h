#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include "clientmanager.h"

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QThreadPool>
#include <QJsonDocument>
#include <Client/clientoptions.h>

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    inline ClientHandler();
    static ClientHandler *instance();
    void SetHost(QString ip, int port);
    void SetSocket(QTcpSocket* socket);
    void SetUser(QString name);
    void SetClientOptionsObject(ClientOptions *co);
    void SetClientManagerObject(ClientManager *cm);
    void sendMessage(MessageDataType msgData);
    bool connectToHost();
    bool disconnectFromHost();

signals:
    void newMessage(const QString &from, const QString &message);
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);

private slots:
    void disconnected();
    void ready();

private:
    QTcpSocket *socket;
    ClientManager *cm;
    ClientOptions *co;
    QString username;
    QJsonDocument doc;
    int port = 0;
    QHostAddress ip = QHostAddress("0.0.0.0");
    QString msgStr;
};

#endif // CLIENTHANDLER_H
