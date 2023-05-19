#include <QtCore/QCoreApplication>
#include "DocumentProcessor.h"
#include "FileUploader.h"

int main(int argc, char* argv[]) {
    if (argc < 2)
    {
        fprintf(stderr, "usage: example input-file\n");
        fprintf(stderr, "\tinput-file: path of PDF, XPS, CBZ or EPUB document to open\n");
        return EXIT_FAILURE;
    }

    QCoreApplication app(argc, argv);  // ��� QtUploader

    char* input = argv[1];
    DocumentProcessor dp(input);

    // ��������� � �������� ������ �������� ����� Qt
    std::vector<QString> fileNames;
    for (int i = 0; i < dp.getPageCount(); ++i) {
        fileNames.push_back(dp.processPage(i)); // ��� processPage ���������� ��� �����
    }

    FileUploader qtUploader(fileNames);
    qtUploader.uploadFiles();

    return app.exec(); // ��� QtUploader
}