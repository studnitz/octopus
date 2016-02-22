#include "guiinterface.h"

GUIInterface::GUIInterface(QObject *parent) : QObject(parent) {}

void GUIInterface::readJson(const QJsonObject &json) {}

QJsonObject GUIInterface::writeJson() {}
