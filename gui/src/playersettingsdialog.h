#ifndef PLAYERSETTINGSDIALOG_H
#define PLAYERSETTINGSDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "ui_playersettingsdialog.h"
#include "mainwindow.h"

namespace Ui {
class PlayerSettingsDialog;
}

/**
 * @brief The PlayerSettingsDialog class is beeing used to display and change
 * the settings of the individual video players (at the moment only their
 * position in the display-grid)
 * @author Bartosz Milejski, Yannick Schädle, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class PlayerSettingsDialog : public QDialog {
  Q_OBJECT

 public:
  /**
   * @brief Constructor. playerIndex again is not to be mistaken with
   * VideoFile.id .
   * @param playerIndex The index of the VideoPlayer-object that the settings
   * are opened on
   * @param parent Parent should be specified for display and memory management
   * reasons. Recommended parent is the videoPlayer that the settings are opened
   * on
   */
  explicit PlayerSettingsDialog(quint8 playerIndex, QWidget *parent = 0);

  ~PlayerSettingsDialog();

 private slots:

  /**
   * @brief on_buttonBox_accepted gets fired when the "Accept"-Button is
   * clicked. The intention is "Save changes". on_buttonBox_accepted basically
   * then applies the changes made to the grid, deletes all players and reprints
   * them for the changes to take effect.
   */
  void on_buttonBox_accepted();

 private:
  /**
   * @brief ui is a pointer to the UI used.
   */
  Ui::PlayerSettingsDialog *ui;

  /**
   * @brief playerIndex has to contain a playerIndex of an existing VideoPlayer
   * object (otherwise it's not clear what player to apply the changes to).
   */
  quint8 playerIndex;

  /**
   * @brief changesDone is beeing set if any changes were made. In that case,
   * when clicking Abort a MessageBox shows asking the user if he is sure to
   * discard the changes made.
   */
  bool changesDone;
};

#endif  // PLAYERSETTINGSDIALOG_H
