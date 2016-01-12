#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../server/src/myserver.h"
#include "../server/src/mythread.h"
#include <QHostInfo>

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MyServer* server;

private slots:
    void on_recordButton_clicked();

    void on_pushButton_2_clicked();

    void on_playButton_clicked();

    void on_openFileButton_clicked();

    void on_stopButton_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

    void videoPlayerClicked(int index);

private:
    Ui::MainWindow *ui;
    QList<QString> *clients;
    void recordStart();
    void recordStop();
    void printClients();
    void log(QString msg);
};

#endif // MAINWINDOW_H
