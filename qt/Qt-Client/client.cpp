#include "client.h"
#include <QHostAddress>

Client::Client(QObject *parent) : QObject(parent)
{
    if(!this->connect(&client, SIGNAL(connected()),this, SLOT(startTransfer())))
    {
        qDebug() << "Could not start client";
    }
    else
    {
        qDebug() << "Client started";
    }


}

Client::~Client()
{
  client.close();
}

void Client::start(QString address, quint16 port)
{
  QHostAddress addr(address);
  client.connectToHost(addr, port);
}

void Client::startTransfer()
{
 client.write("Hello, world", 12);
}
