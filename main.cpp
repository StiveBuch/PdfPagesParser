#include <QCoreApplication>
#include "DocumentProcessor.h"
#include "FileUploader.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: program input-file\n");
        fprintf(stderr, "\tinput-file: path of PDF, XPS, CBZ or EPUB document to open\n");
        return EXIT_FAILURE;
    }
    QCoreApplication app(argc, argv);
    DocumentProcessor docProc(argv[1]);
    if (!docProc.processDocument()) {
        return EXIT_FAILURE;
    }

    FileUploader uploader(docProc.getOutputFiles());
    QObject::connect(&uploader, &FileUploader::allFilesUploaded, &app, &QCoreApplication::quit);
    uploader.uploadFiles();

    return app.exec();
}
