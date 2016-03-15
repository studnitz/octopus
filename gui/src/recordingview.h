#ifndef RECORDINGVIEW_H
#define RECORDINGVIEW_H

#include <QObject>
#include <QtGui>
#include <QWidget>
#include <QPushButton>

/**
 * @brief RecordingView class
 * Class for the recording tab.
 */
class RecordingView : public QObject {
  Q_OBJECT
 public:
  explicit RecordingView(QObject* parent = 0, QWidget* tab = 0);
  QWidget* tab;
  /**
   * @brief record_button triggered when button is clicked
   * @param recordButton pointer to button
   */
  void record_button(QPushButton* recordButton);
  /**
   * @brief recordStart starts to record
   */
  void recordStart();
  /**
   * @brief recordStop stops recording
   */
  void recordStop();
 signals:

 public slots:
};

#endif  // RECORDINGVIEW_H
