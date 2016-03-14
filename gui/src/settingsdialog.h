#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
  Q_OBJECT

  /**
   * @brief settings to store settings
   * settings-file is in ~/.config/TUD\ IAD/octopus
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
