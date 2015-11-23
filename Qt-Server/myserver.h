#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>

class MyServer : public QTcpServer
{
public:
    MyServer();
};

#endif // MYSERVER_H
