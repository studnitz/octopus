#include "recordinggridelement.h"

#include <QMouseEvent>
#include <QMenu>

RecordingGridElement::RecordingGridElement(QWidget *parent) : QLabel(parent)
{

}

void RecordingGridElement::contextMenuEvent(QContextMenuEvent *ev) {

  QMenu contextMenu;

  QAction *selected = contextMenu.exec(ev->globalPos());

  if (selected) {

    }
}
