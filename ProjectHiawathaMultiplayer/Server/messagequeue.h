#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <QObject>
#include <QQueue>
#include <QDebug>

class MessageQueue : public QObject
{
    Q_OBJECT

public:
    inline MessageQueue() {}
    static MessageQueue* instance();

    static void put(QString str);
    static QString get();
    static bool isEmpty();

private:

signals:
    void MessageReady();

};

static QQueue<QString> messages;
#endif // MESSAGEQUEUE_H
