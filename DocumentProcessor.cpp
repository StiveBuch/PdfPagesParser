#include "DocumentProcessor.h"

#include <stdio.h>
#include <stdlib.h>

DocumentProcessor::DocumentProcessor(const QString& inputFile, float zoom, float rotate)
    : inputFile(inputFile), zoom(zoom), rotate(rotate) {

    ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (!ctx) {
        fprintf(stderr, "cannot create mupdf context\n");
        exit(EXIT_FAILURE);
    }

    fz_try(ctx)
        fz_register_document_handlers(ctx);
    fz_catch(ctx)
    {
        fprintf(stderr, "cannot register document handlers: %s\n", fz_caught_message(ctx));
        fz_drop_context(ctx);
        exit(EXIT_FAILURE);
    }

    fz_try(ctx)
        doc = fz_open_document(ctx, inputFile.toStdString().c_str());
    fz_catch(ctx)
    {
        fprintf(stderr, "cannot open document: %s\n", fz_caught_message(ctx));
        fz_drop_context(ctx);
        exit(EXIT_FAILURE);
    }
}

DocumentProcessor::~DocumentProcessor() {
    cleanUp();
}

void DocumentProcessor::cleanUp() {
    if (doc) {
        fz_drop_document(ctx, doc);
        doc = nullptr;
    }

    if (ctx) {
        fz_drop_context(ctx);
        ctx = nullptr;
    }
}

bool DocumentProcessor::processDocument() {
    int page_count = 0;
    fz_try(ctx)
        page_count = fz_count_pages(ctx, doc);
    fz_catch(ctx)
    {
        fprintf(stderr, "cannot count number of pages: %s\n", fz_caught_message(ctx));
        return false;
    }

    for (int i = 0; i < page_count; i++) {
        fz_pixmap* pix;
        fz_try(ctx)
            pix = fz_new_pixmap_from_page_number(ctx, doc, i, fz_scale(zoom / 100, zoom / 100), fz_device_rgb(ctx), 0);
        fz_catch(ctx)
        {
            fprintf(stderr, "cannot render page: %s\n", fz_caught_message(ctx));
            return false;
        }

        // Convert fz_pixmap to FreeImage FIBITMAP
        FIBITMAP* bitmap = createFreeImageBitmap(pix);
        if (!bitmap) {
            fprintf(stderr, "cannot convert pixmap to FreeImage bitmap\n");
            fz_drop_pixmap(ctx, pix);
            return false;
        }

        // Resize the image to 128x128
        FIBITMAP* resized = resizeFreeImageBitmap(bitmap);
        if (!resized) {
            fprintf(stderr, "cannot resize image\n");
            FreeImage_Unload(bitmap);
            fz_drop_pixmap(ctx, pix);
            return false;
        }

        // Save the image data to a PNG file.
        char output_filename[256];
        sprintf(output_filename, "output_page_%d.png", i + 1);  // Create a unique filename for each page
        if (!FreeImage_Save(FIF_PNG, resized, output_filename, 0)) {
            fprintf(stderr, "cannot save image: %s\n", output_filename);
            FreeImage_Unload(resized);
            FreeImage_Unload(bitmap);
            fz_drop_pixmap(ctx, pix);
            return false;
        }

        outputFiles.push_back(QString::fromStdString(output_filename));

        // Clean up.
        FreeImage_Unload(resized);
        FreeImage_Unload(bitmap);
        fz_drop_pixmap(ctx, pix);
    }

    return true;
}

std::vector<QString> DocumentProcessor::getOutputFiles() const {
    return outputFiles;
}
int DocumentProcessor::getPageCount() {
    int page_count = 0;
    fz_try(ctx)
        page_count = fz_count_pages(ctx, doc);
    fz_catch(ctx)
    {
        fprintf(stderr, "cannot count number of pages: %s\n", fz_caught_message(ctx));
    }
    return page_count;
}
