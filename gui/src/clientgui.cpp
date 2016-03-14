#include "clientgui.h"

ClientGui::ClientGui(QString ip, QString name, float cpu, float mem,
                     float disk) {
  this->ip = ip;
  this->cpu = cpu;
  this->mem = mem;
  this->disk = disk;
  this->name = name;
}
