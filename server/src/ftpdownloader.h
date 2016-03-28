#ifndef FTPDOWNLOADER_H
#define FTPDOWNLOADER_H

#include <QUrl>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

/**
 * @brief The FtpDownloader class is responsible for fetching the recorded data
 * stored on the clients after a recording. The client has to run an FTP-Server
 * and the Server has to get access rights as well (currently realized by
 * allowing total access for anyone on the root-directory of the client).
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class FtpDownloader : public QObject {
  Q_OBJECT
 public:
  FtpDownloader(QObject *parent = 0, QUrl ftpUrl = QUrl(""), QString path = "");
  double progress() const;
  void startDownload();
 signals:

 public slots:

 private slots:
  void readyRead();
  void finished();
  void updateDownloadProgress(qint64 const bytesRead, qint64 const totalBytes);

 private:
  QUrl ftpUrl;
  QString path;
  qint64 bytes;
  qint64 total;
  QNetworkReply *reply;
  QNetworkAccessManager *nam;
  QFile *file;
};

#endif  // FTPDOWNLOADER_H
