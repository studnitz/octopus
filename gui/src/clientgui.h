#ifndef CLIENTGUI_H
#define CLIENTGUI_H

#include <QObject>
#include <QVector>
/**
 * @brief The ClientGui class
 * Class for representation and storage of clientinfo for the GUI.
 * HAS NO TCP-COMMUNICATION-FUNCTIONALITY
 */
class ClientGui
{
public:
    ClientGui(QString ip, QList<float> info);
    QString ip;
    QList<float> clientInfo;
};

#endif // CLIENTGUI_H
