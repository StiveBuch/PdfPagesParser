#pragma once
#include <mupdf/fitz.h>
#include <FreeImage.h>
#include <QString>
#include <vector>

class DocumentProcessor {
public:
    DocumentProcessor(const QString& inputFile, float zoom = 100, float rotate = 0);
    ~DocumentProcessor();
    bool processDocument();

    std::vector<QString> getOutputFiles() const;
    int getPageCount();

private:
    fz_context* ctx = nullptr;
    fz_document* doc = nullptr;
    QString inputFile;
    float zoom;
    float rotate;
    std::vector<QString> outputFiles;

    void cleanUp();
    FIBITMAP* createFreeImageBitmap(fz_pixmap* pixmap);
    FIBITMAP* resizeFreeImageBitmap(FIBITMAP* bitmap);
    FIBITMAP* resizeImageToThumbnail(FIBITMAP* bitmap,int width);
};

