#ifndef RECORDING_H
#define RECORDING_H

#include "grid.h"
#include <QObject>
#include <QDateTime>

class Recording : public QObject
{
  Q_OBJECT
public:
  explicit Recording(QObject *parent = 0);
  Recording(QDateTime datetime, Grid grid);
  QDateTime datetime;
  Grid grid;

signals:

public slots:
};

#endif // RECORDING_H
