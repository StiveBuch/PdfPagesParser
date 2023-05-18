#include "CurlUploader.h"
#include <iostream>


CurlUploader::CurlUploader(const std::vector<QString>& files)
    : files(files)
{
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
}

CurlUploader::~CurlUploader()
{
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

bool CurlUploader::uploadFiles() {
    if (!curl) {
        std::cerr << "Curl initialization failed\n";
        return false;
    }

    for (const auto& file : files) {
        std::string url = "http://example.com/upload";  // Set your upload URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        struct curl_httppost* post = nullptr;
        struct curl_httppost* last = nullptr;
        curl_formadd(&post, &last,
            CURLFORM_COPYNAME, "fileupload",
            CURLFORM_FILE, file.toStdString().c_str(),
            CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
            curl_formfree(post);
            return false;
        }
        curl_formfree(post);
    }

    return true;
}
