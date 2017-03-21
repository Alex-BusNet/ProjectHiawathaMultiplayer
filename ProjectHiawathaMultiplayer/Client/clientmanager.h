#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QWidget>
#include <Common/nation.h>

class ClientManager : public QWidget
{
    Q_OBJECT
public:
    explicit ClientManager(QWidget *parent = 0, bool fullscreen = false, Nation player = India,
                           QString IP = "127.0.0.1", QString username = "Sparta");
    ~ClientManager();

private:

};

#endif // CLIENTMANAGER_H
