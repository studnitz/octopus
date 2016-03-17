#ifndef FTPDOWNLOADER_H
#define FTPDOWNLOADER_H

#include <QUrl>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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
