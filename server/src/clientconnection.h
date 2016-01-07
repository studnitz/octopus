#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QDebug>
class ClientConnection : public QObject {
  Q_OBJECT
 public:
  explicit ClientConnection(QObject *parent = 0);
  ~ClientConnection();
 signals:

 public slots:
};

#endif  // CLIENTCONNECTION_H
