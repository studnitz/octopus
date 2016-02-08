#ifndef PLAYERSETTINGSDIALOG_H
#define PLAYERSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
  class PlayerSettingsDialog;
}

class PlayerSettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PlayerSettingsDialog(quint8 playerIndex, QWidget *parent = 0);
  ~PlayerSettingsDialog();

private:
  Ui::PlayerSettingsDialog *ui;
  quint8 playerIndex;
};

#endif // PLAYERSETTINGSDIALOG_H
