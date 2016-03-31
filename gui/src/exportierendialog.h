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
  explicit ExportierenDialog(QWidget *parent = 0);
  ~ExportierenDialog();

 public slots:
  void exportFinished();

  void exportErrored();
private slots:
  void on_startButton_clicked();
  void new_progress();

 private:
  Ui::ExportierenDialog *ui;
  QTimer *timer;
};

#endif  // EXPORTIERENDIALOG_H
