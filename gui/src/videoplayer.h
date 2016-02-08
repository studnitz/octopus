#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QVideoWidget>

class VideoPlayer : public QVideoWidget {
  Q_OBJECT

 public:
  VideoPlayer(quint8 index, quint32 videoFileId, QWidget *parent = 0);

  /**
   * @brief contextMenuEvent Event handler for context-menu opening.
   * @param e Details about the event
   */
  void contextMenuEvent(QContextMenuEvent *e);

  /**
   * @brief index Unique ID of the player.
   */
  quint8 index;

  /**
   * @brief videoFileId ID of the Videofile that VideoPlayer is supposed to play
   */
  quint32 videoFileId;

 signals:
  /**
   * @brief playerOpenOptions Fired when 'Einstellungen' in the context menu is
   * clicked.
   * @param index ID of the player
   */
  void playerOpenOptions(quint8 index);
  /**
   * @brief playerDelete Fired when 'Player löschen' in the context menu is
   * clicked.
   * @param index ID of the player
   */
  void playerDelete(quint8 index);
};

#endif  // VIDEOPLAYER_H
