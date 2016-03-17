#ifndef RECORDINGVIEW_H
#define RECORDINGVIEW_H

#include <QObject>
#include <QtGui>
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QGridLayout>
#include "../server/src/recording.h"

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
  /**
   * @brief updateGrid is used to update the recording grid
   * This method should be called whenever the device video list is updated
   */
  void updateGrid();
  /**
   * @brief updateVideoDevices updates the list of available video devices
   */
  void updateVideoDevices();

  /**
   * @brief createRecording creates a Recording from the current gridLayout and
   * saves it as a *.off-file
   */
  void createRecording();
 signals:

 public slots:

 private:
  /**
   * @brief dimension the grid dimension (dimension x dimension)
   */
  quint16 dimension;
  /**
   * @brief rec The recording we manipulate to create a *.off-file
   */
  Recording* rec;
  /**
   * @brief gridLayout the grid used for rendering the recording view grid
   */
  QGridLayout* gridLayout;
  /**
   * @brief videoDeviceList is the list of currently available video devices
   */
  QListWidget* videoDeviceList;
};

#endif  // RECORDINGVIEW_H
