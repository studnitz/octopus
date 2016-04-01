#include "ftpdownloader.h"
#include <QDir>

FtpDownloader::FtpDownloader(QObject *parent, QUrl ftpUrl, QString path)
    : QObject(parent), ftpUrl(ftpUrl), path(path) {}

void FtpDownloader::startDownload() {
  nam = new QNetworkAccessManager();
  reply = nam->get(QNetworkRequest(ftpUrl));
  connect(reply, &QNetworkReply::readyRead, this, &FtpDownloader::readyRead);
  connect(reply, &QNetworkReply::downloadProgress, this,
          &FtpDownloader::updateDownloadProgress);
  connect(reply, &QNetworkReply::finished, this, &FtpDownloader::finished);

  file = new QFile(path);
  qDebug() << "FtpDownload:  from: " + ftpUrl.toString() +  " Writing to: " + path;
  if (!file->open(QIODevice::WriteOnly)) {
    qWarning() << "Couldn't open file to download to";
  }

}

void FtpDownloader::readyRead() {
  if (file) {
    file->write(reply->readAll());
  }
}

void FtpDownloader::updateDownloadProgress(qint64 const bytesRead,
                                           qint64 const totalBytes) {
  bytes = bytesRead;
  total = totalBytes;
}

double FtpDownloader::progress() const {
  double result = bytes / total;
  return result * 100;
}

void FtpDownloader::finished() {
  qDebug() << "FtpDownloader: finished";
  file->flush();
  file->close();
  reply->deleteLater();
  reply = 0;
  bytes = 0;
  total = 0;

  QDir currentDir = QDir::current();
  currentDir.cd("recordings");
  QString relativePath = currentDir.relativeFilePath(path);
  qDebug() << "ftpDownloader relative path: " << relativePath;

  emit isFinished(relativePath);
}
