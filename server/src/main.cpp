#include <QCoreApplication>

#include "server.h"
#include "serverinterface.h"

/**
 * @brief main starts the server.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);
  Server server;
  server.startServer();
  ServerInterface serverInterface;
  serverInterface.setServer(&server);
  serverInterface.start();
  return a.exec();
}
