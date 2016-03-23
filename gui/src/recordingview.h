#ifndef RECORDINGVIEW_H
#define RECORDINGVIEW_H

#include <QPushButton>
#include <QListWidget>
#include <QGridLayout>
#include "../server/src/recording.h"

/**
 * @brief The RecordingView class stores the functionality from MainWindow that
 * is needed for the whole recording-tab. It was once introduced to make
 * MainWindow handier and more overseeable. For a more detailed description of the
 * methods go to the MainWindow-class where a method with the same name for each
 * method of this class exists.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class RecordingView : public QObject {
  Q_OBJECT
 public:
  /**
   * @brief Constructor for RecordingView. Nothing special.
   * @param parent obvious, isn't it?
   * @param tab a pointer to the recording-tab to be able to create widgets in
   * it more easily
   */
  explicit RecordingView(QObject* parent = 0, QWidget* tab = 0);

  /**
   * @brief tab stores a pointer to the recording-tab to be able to create
   * widgets in there easily.
   */
  QWidget* tab;

  /**
   * @brief record_button triggered when button is clicked
   * @param recordButton pointer to button
   */
  void record_button(QPushButton* recordButton);
  /**
   * @brief recordStart starts to record. More precisely it sends data via
   * GuiInterface to the Server to make the clients start recording.
   */
  void recordStart();
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
   * @brief recordStop stops to record. More precisely it sends data via
   * GuiInterface to the Server to make the clients stop recording.
   */
  void recordStop();

  void createRecording();
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
