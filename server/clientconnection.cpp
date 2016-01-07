#include "clientconnection.h"

ClientConnection::ClientConnection(qintptr socketDescriptor, QObject *parent)
    : QObject(parent) {
  this->socketDescriptor = socketDescriptor;
}

ClientConnection::~ClientConnection() {
  qDebug() << "Client Connection terminated";
}

void ClientConnection::sendCommand(int value) {
    QByteArray message;
    message.setNum(value);
    qDebug() << message;
    socket->write(message);
    socket->waitForReadyRead();
    message = socket->readLine();
    qDebug()<< message;
}



void ClientConnection::setUpSocket() {
  socket = new QTcpSocket();
  socket->setSocketDescriptor(socketDescriptor);
 // qDebug() << "Client connection established";
}
