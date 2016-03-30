#include <QCoreApplication>
#include "server.h"
#include "src/serverinterface.h"
#include "gst_exporter.h"

/**
 * @brief main starts the server.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);

  Recording* rec = new Recording();
  rec->loadRecording("recordings/2016_03_29_15_53_32.off");
  Server server;
  server.startServer();
  ServerInterface serverInterface;
  serverInterface.setServer(&server);
  serverInterface.start();

  GstExporter* exporter = new GstExporter(rec);

  exporter->exportVideo();

  return a.exec();
}
