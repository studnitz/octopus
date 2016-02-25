#include "clientgui.h"

ClientGui::ClientGui(QString ip, QVector<float> info) {
  this->ip = ip;
  this->clientInfo = info;
}
