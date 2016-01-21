#ifndef GRID_H
#define GRID_H

#include <QObject>

class Grid : public QObject
{
  Q_OBJECT
public:
  explicit Grid(QObject *parent = 0);

signals:

public slots:
};

#endif // GRID_H