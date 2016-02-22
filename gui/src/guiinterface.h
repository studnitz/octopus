#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>

class GUIInterface : public QObject {
  Q_OBJECT
 public:
  explicit GUIInterface(QObject *parent = 0);

 signals:

 public slots:

 private:
  void readJson(const QJsonObject &json);
  QJsonObject writeJson();
};

#endif  // GUIINTERFACE_H
