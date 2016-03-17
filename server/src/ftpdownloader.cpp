#include "ftpdownloader.h"

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
  file->open(QIODevice::WriteOnly);
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
  file->flush();
  file->close();
  reply->deleteLater();
  reply = 0;
  bytes = 0;
  total = 0;
}
