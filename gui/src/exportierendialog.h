#ifndef EXPORTIERENDIALOG_H
#define EXPORTIERENDIALOG_H

#include <QDialog>
#include <QTimer>

#include "ui_exportierendialog.h"
#include "mainwindow.h"

namespace Ui {
class ExportierenDialog;
}

/**
 * @brief The ExportierenDialog class represents the Dialog that opens when
 * you'd want to export a Recording. It has a progress-bar that is fed with
 * actual information.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class ExportierenDialog : public QDialog {
  Q_OBJECT

 public:
  /**
   * @brief ExportierenDialog creates a new export dialog
   * @param parent the parent object
   */
  explicit ExportierenDialog(QWidget *parent = 0);
  ~ExportierenDialog();

 public slots:
  /**
   * @brief exportFinished is called when the export is finished
   */
  void exportFinished();

  /**
   * @brief exportErrored is called when the export encountered an error
   */
  void exportErrored();

 private slots:
  /**
   * @brief on_startButton_clicked startes the video export by sending the
   * server a signal to start it
   */
  void on_startButton_clicked();

  /**
   * @brief new_progress is called when the progress has updated
   */
  void new_progress();

 private:
  /**
   * @brief ui the ui we use
   */
  Ui::ExportierenDialog *ui;

  /**
   * @brief timer timer used for updating the progress and status
   */
  QTimer *timer;
};

#endif  // EXPORTIERENDIALOG_H
