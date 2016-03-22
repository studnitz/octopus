#ifndef FTPDOWNLOADER_H
#define FTPDOWNLOADER_H

#include <QUrl>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class FtpDownloader : public QObject {
  Q_OBJECT
 public:
  /**
   * @brief FtpDownloader is used to download the video files from the clients
   * @param parent is the parent QObject
   * @param ftpUrl the QUrl which should be downloaded from
   * @param path the path where the downloaded file should be saved
   */
  FtpDownloader(QObject *parent = 0, QUrl ftpUrl = QUrl(""), QString path = "");

  /**
   * @brief progress the current download progress
   * @return the progress between 0 and 1
   */
  double progress() const;
  /**
   * @brief startDownload starts the download
   */
  void startDownload();
 signals:

 public slots:

 private slots:
  /**
   * @brief readyRead is connected to the readyRead() signal of the
   * QNetworkReply
   */
  void readyRead();
  /**
   * @brief finished is connected to the finished() signal of the QNetworkReply
   */
  void finished();
  /**
   * @brief updateDownloadProgress is connected to the updateDownloadProgress
   * signal of the QNetworkReply
   * @param bytesRead are the bytes read so far
   * @param totalBytes are the total bytes of the file
   */
  void updateDownloadProgress(qint64 const bytesRead, qint64 const totalBytes);

 private:
  /**
   * @brief ftpUrl the url which we download from
   */
  QUrl ftpUrl;
  /**
   * @brief path the path we save the file to
   */
  QString path;
  /**
   * @brief bytes the bytes currently written to path
   */
  qint64 bytes;
  /**
   * @brief total the total bytes of the file
   */
  qint64 total;
  /**
   * @brief reply the QNetworkReply we work with
   */
  QNetworkReply *reply;
  /**
   * @brief nam the QNetworkAccessManager we work with
   */
  QNetworkAccessManager *nam;
  /**
   * @brief file the QFile we use to write to path
   */
  QFile *file;
};

#endif  // FTPDOWNLOADER_H
