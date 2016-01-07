#include "mythread.h"
#include "myserver.h"

MyThread::MyThread(qintptr ID, QObject *parent) : QThread(parent) {
  this->socketDescriptor = ID;
}

void MyThread::run() {
  // thread starts here
  qDebug() << " Thread started";

  socket = new QTcpSocket();

  // set the ID
  if (!socket->setSocketDescriptor(this->socketDescriptor)) {
    // something's wrong, we just emit a signal
    emit error(socket->error());
    return;
  }

  // connect socket and signal
  // note - Qt::DirectConnection is used because it's multithreaded
  //        This makes the slot to be invoked immediately, when the signal is
  //        emitted.
  qDebug() << socket->bytesAvailable();
//  connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()),
//          Qt::DirectConnection);
  connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

  // We'll have multiple clients, we want to know which is which
  qDebug() << socketDescriptor
           << "        IP:" << socket->peerAddress().toString()
           << " Client connected";

  // make this thread a loop,
  // thread will stay alive so that signal/slot to function properly
  // not dropped out in the middle when thread dies

  exec();
}

void MyThread::readyRead() {
  QByteArray info = socket->read(3);
  qDebug() << socket->bytesAvailable();

  qDebug() << socketDescriptor
           << "        IP:" << socket->peerAddress().toString()
           << "       Data in: " << info;

}

void MyThread::sendOrder() {

    socket->write("000");
  //socket->write(*command);
//  socket->waitForReadyRead();
//  QByteArray data = socket->readLine();
//  QString answer;
//  answer.append(data);

  qDebug() << "ORDER Send";
  //return &answer;
}

void MyThread::disconnected() {
  qDebug() << socketDescriptor << " Disconnected";

  socket->deleteLater();
  exit(0);
}
