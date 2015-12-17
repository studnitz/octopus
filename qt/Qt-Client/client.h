#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>

#include <QtNetwork>
#include <QTcpSocket>
#include <QDebug>
#include <QString>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    void start(QString address, quint16 port);
signals:

public slots:
    void startTransfer();
    QTcpSocket::SocketState getState() const;
private:
   QTcpSocket client;
};

#endif // TCPCLIENT_H
