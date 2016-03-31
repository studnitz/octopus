#ifndef RECORDINGVIEW_H
#define RECORDINGVIEW_H

#include <QPushButton>
#include <QListWidget>
#include <QStringListModel>
#include <QListView>
#include <QGridLayout>
#include <QLabel>

#include "recordinggrid.h"

#include "../server/src/recording.h"

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
   * @brief recordStop stops to record. More precisely it sends data via
   * GuiInterface to the Server to make the clients stop recording.
   */
  void recordStop();

  /**
   * @brief createRecording creates a Recording object and sends a signal to the
   * server to start the recording with the current grid
   */
  void createAndStartRecording();

 public slots:

  /**
   * @brief updateVideoDeviceList updates the video device list in the GUI
   * @param list the updated list
   */
  void updateVideoDeviceList(QStringList list);
  void updateGrid2();

  /**
   * @brief createEmptyEntry creates an empty grid entry
   * @return the empty grid entry
   */
  QLabel* createEmptyEntry() const;

  /**
   * @brief createEntry creates a normal entry
   * @param entry a grid element with the device name
   * @return the grid element
   */
  QLabel* createEntry(RecordingGrid::GridElement* entry) const;

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
   * @brief videoDeviceListView the list view used to display the video device
   * list
   */
  QListView* videoDeviceListView;

  /**
   * @brief videoDeviceListModel the model used by the device list view
   */
  QStringListModel* videoDeviceListModel;

  /**
   * @brief recGrid the grid representing the current video device elements
   */
  RecordingGrid* recGrid;
};

#endif  // RECORDINGVIEW_H
