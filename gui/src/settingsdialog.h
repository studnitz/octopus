#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>

#include "ui_settingsdialog.h"

namespace Ui {
class SettingsDialog;
}

/**
 * @brief The SettingsDialog class is used for creating the SettingsDialog,
 * handling events occuring there as well as storing and retrieving settings
 * made there.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class SettingsDialog : public QDialog {
  Q_OBJECT

  /**
   * @brief settings stores the actual settings. The settings file on linux is
   * stored in ~/.config/TUD\ IAD/octopus. On Windows it should be the same
   * except ~ is your win home-directory.
   */
  QSettings *settings = new QSettings(QString("TUD IAD"), QString("octopus"));

 public:
  explicit SettingsDialog(QWidget *parent = 0);
  ~SettingsDialog();

 private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

  void on_recordStorage_activated(int index);

 private:
  Ui::SettingsDialog *ui;
};

#endif  // SETTINGSDIALOG_H
