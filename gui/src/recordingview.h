#ifndef RECORDINGVIEW_H
#define RECORDINGVIEW_H

#include <QPushButton>

/**
 * @brief The RecordingView class stores the functionality from MainWindow that
 * is needed for the whole recording-tab. It was once introduced to make
 * MainWindow handier and more overseeable. For a more detailed description of
 * the
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
   * @brief recordStart starts to record. More precisely it sends data via
   * GuiInterface to the Server to make the clients start recording.
   */
  void recordStart();

  /**
   * @brief recordStop stops to record. More precisely it sends data via
   * GuiInterface to the Server to make the clients stop recording.
   */
  void recordStop();

 public slots:

  /**
   * @brief record_button is triggered when the Record-Button is clicked
   * @param recordButton pointer to the clicked button
   */
  void record_button(QPushButton* recordButton);
};

#endif  // RECORDINGVIEW_H
