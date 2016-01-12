#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

//#include <QObject>
#include <QVideoWidget>

class VideoPlayer : public QVideoWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent = 0, int index = 0);
    void mousePressEvent(QMouseEvent *e);

private:
    int index;

signals:
    void playerClicked(int index);
};

#endif // VIDEOPLAYER_H
