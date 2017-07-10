#include "messagequeue.h"

Q_GLOBAL_STATIC(MessageQueue, mq)

MessageQueue *MessageQueue::instance()
{
    return mq;
}

void MessageQueue::put(QString str)
{
    messages.enqueue(str);
    emit(MessageQueue::instance()->MessageReady());
}

QString MessageQueue::get()
{
    return messages.dequeue();
}

bool MessageQueue::isEmpty()
{
    return messages.isEmpty();
}
