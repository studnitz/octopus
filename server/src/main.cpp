#include <QCoreApplication>
#include "server.h"
#include "src/serverinterface.h"
Server server;
ServerInterface* serverInterface;

/**
 * @brief main starts the server.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander
 * Studnitz
 * @copyright GNU Public Licence.
 * @date 31.03.2016
 */
int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);
  server.startServer();

  serverInterface = new ServerInterface(0);
  serverInterface->setServer(&server);
  serverInterface->start();

  delete serverInterface;
  return a.exec();
}
