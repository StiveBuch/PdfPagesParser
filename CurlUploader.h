#pragma once
#ifndef CURL_UPLOADER_H
#define CURL_UPLOADER_H

#include <curl/curl.h>
#include <QStringList>
#include <vector>

class CurlUploader {
public:
    CurlUploader(const std::vector<QString>& files);
    ~CurlUploader();

    bool uploadFiles();

private:
    CURL* curl = nullptr;
    std::vector<QString> files;
};

#endif // CURL_UPLOADER_H
