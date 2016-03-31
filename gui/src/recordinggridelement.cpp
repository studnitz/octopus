#include "recordinggridelement.h"

RecordingGridElement::RecordingGridElement(QWidget *parent) : QLabel(parent)
{

}

void RecordingGridElement::contextMenuEvent(QContextMenuEvent *ev) {

  QMenu contextMenu;

  QAction *selected = contextMenu.exec(ev->globalPos());

  if (selected) {

    }
}
