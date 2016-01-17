#include "videoplayer.h"
#include <QMouseEvent>

VideoPlayer::VideoPlayer(QWidget *parent, int index) :
    QVideoWidget(parent)
{
    this->index = index;
}

void VideoPlayer::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        emit playerClicked(this->index);
    }
}
