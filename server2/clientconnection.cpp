#include "clientconnection.h"

ClientConnection::ClientConnection(qintptr socketDescriptor ,QObject *parent) : QObject(parent)
{
    this->socketDescriptor=socketDescriptor;


}

ClientConnection::~ClientConnection(){

    qDebug() << "Client Connection terminated";

}

void ClientConnection::writeData(){
    socket=new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);
    qDebug() << socket->thread();
    qDebug() << "write Command";
    qDebug() << socket->isWritable();
    qDebug() << socket->isValid();


    socket->write("0");
}
