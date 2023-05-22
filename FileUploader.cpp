#include "FileUploader.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFile>

FileUploader::FileUploader(const std::vector<QString>& files, QObject* parent)
    : QObject(parent), files(files)
{
    connect(&networkManager, &QNetworkAccessManager::finished, this, &FileUploader::onUploadFinished);
}

void FileUploader::uploadFiles(QUrl url)
{
    for (const auto& file : files)
    {
        QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + file + "\""));
        QFile* fileObj = new QFile(file);
        fileObj->open(QIODevice::ReadOnly);
        filePart.setBodyDevice(fileObj);
        fileObj->setParent(multiPart);

        multiPart->append(filePart);

        QNetworkRequest request(url);
        QNetworkReply* reply = networkManager.post(request, multiPart);
        multiPart->setParent(reply);
    }
}

void FileUploader::onUploadFinished(QNetworkReply* reply)
{
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError)
    {
        // Handle the error
        return;
    }

    // Continue with the next file
    uploadedCount++;
    if (uploadedCount == files.size())
    {
        emit allFilesUploaded();
    }
}
