#ifndef RECORDINGGRID_H
#define RECORDINGGRID_H

#include <QObject>
#include <QList>

class RecordingGrid : public QObject {
  Q_OBJECT
 public:
  explicit RecordingGrid(quint8 width = 3, quint8 height = 3,
                         QObject* parent = 0);

  typedef struct {
    quint16 id;
    QString hostname;
    QString device;
  } GridElement;

  typedef QList<GridElement> GridRow;
  typedef QList<GridRow> GridMatrix;

  inline int height() const { return height_; }
  inline int width() const { return width_; }

 signals:
  void entryUpdated(quint8 x, quint8 y);

 public slots:
  void addRow();
  void addColumn();
  void removeColumn();
  void removeRow();

  void updateEntry(quint8 x, quint8 y, GridElement entry);
private:
  quint8 height_;
  quint8 width_;
  GridMatrix grid;
};

#endif  // RECORDINGGRID_H
