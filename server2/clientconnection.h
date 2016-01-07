#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    explicit ClientConnection(qintptr socketDescriptor ,QObject *parent = 0);
    ~ClientConnection();
signals:

public slots:
    void writeData();

protected:
    qintptr socketDescriptor;
    QTcpSocket* socket;
};

#endif // CLIENTCONNECTION_H
