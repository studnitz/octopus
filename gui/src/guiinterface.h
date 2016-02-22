#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QHostAddress>

class GUIInterface : public QObject {
  Q_OBJECT
 public:
  explicit GUIInterface(QHostAddress destAddr, quint16 port,
                        QObject *parent = 0);
  void sendData(QString &str);
 signals:

 public slots:

 private:
  void readJson(const QJsonObject &json);
  void writeJson();
  QTcpSocket *socket;
};

#endif  // GUIINTERFACE_H
