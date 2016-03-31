#include "clientgui.h"

ClientGui::ClientGui(QString ip, QString name, float cpu, float mem, float disk,
                     QStringList devices, QString currentTime) {
  this->ip = ip;
  this->cpu = cpu;
  this->mem = mem;
  this->disk = disk;
  this->name = name;
  this->devices = devices;
  this->currentTime = currentTime;
}
