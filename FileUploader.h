#pragma once
#ifndef FILE_UPLOADER_H
#define FILE_UPLOADER_H

#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QObject>
#include <QStringList>
#include <vector>

class FileUploader : public QObject {
    Q_OBJECT
public:
    FileUploader(const std::vector<QString>& files, QObject* parent = nullptr);

    void uploadFiles();

signals:
    void allFilesUploaded();

private slots:
    void onUploadFinished();

private:
    QNetworkAccessManager networkManager;
    std::vector<QString> files;
    int uploadedCount = 0;
};

#endif // FILE_UPLOADER_H
