#ifndef CLIENTGUI_H
#define CLIENTGUI_H

#include <QObject>
#include <QVector>
#include <QStringList>
/**
 * @brief Class for representation and storage of clientinfo for the GUI.
 * HAS NO TCP-COMMUNICATION-FUNCTIONALITY
 */
class ClientGui {
 public:
  /**
 * @brief ClientGui
 * Standard constructor
 * @param ip
 * @param name
 * @param cpu
 * @param mem
 * @param disk
 */
  ClientGui(QString ip, QString name, float cpu, float mem, float disk,
            QStringList devices, QString currentTime);
  /**
   * @brief ip
   * Stores the IP-Address of a client
   */
  QString ip;
  /**
   * @brief name
   * Stores the hostname of a client
   */
  QString name;
  /**
   * @brief cpu
   * Stores the CPU-usage of a client
   */
  float cpu;
  /**
   * @brief mem
   * Stores the memory-usage of a client
   */
  float mem;
  /**
   * @brief disk
   * Stores the disk-usage of a client
   */
  float disk;

  /**
   * @brief devices list of video devices in form of their device paths
   */
  QStringList devices;

  /**
   * @brief currentTime is the current time on the client
   */
  QString currentTime;
};

#endif  // CLIENTGUI_H
