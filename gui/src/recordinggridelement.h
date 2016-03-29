#ifndef RECORDINGGRIDELEMENT_H
#define RECORDINGGRIDELEMENT_H

#include <QLabel>

class RecordingGridElement : public QLabel {
  Q_OBJECT

 public:
  RecordingGridElement(QWidget *parent = 0);
  void contextMenuEvent(QContextMenuEvent *ev);
};

#endif  // RECORDINGGRIDELEMENT_H
