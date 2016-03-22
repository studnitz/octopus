#ifndef EXPORTIERENDIALOG_H
#define EXPORTIERENDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class ExportierenDialog;
}

class ExportierenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportierenDialog(QWidget *parent = 0);
    ~ExportierenDialog();

private slots:
    void on_startButton_clicked();
    void new_progress();

private:
    Ui::ExportierenDialog *ui;
    QTimer* timer;
};

#endif // EXPORTIERENDIALOG_H
