#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;

/*struct TClient {
    QString name;
    QString otherdata;
};*/

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_recordButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QList<QString> *clients;
    void recordStart();
    void recordStop();
    void printClients();
};

#endif // MAINWINDOW_H
