#include "DocumentProcessor.h"

#include <stdio.h>
#include <stdlib.h>
#include <QDebug>

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

    // Calculate the scale for 180 DPI
    float scale_factor = 180.0f / 72.0f;

    for (int i = 0; i < page_count; i++) {
        fz_pixmap* pix;
        fz_try(ctx)
            pix = fz_new_pixmap_from_page_number(ctx, doc, i, fz_scale(scale_factor, scale_factor), fz_device_rgb(ctx), 0);
        fz_catch(ctx)
        {
            fprintf(stderr, "cannot render page: %s\n", fz_caught_message(ctx));
            return false;
        }

#ifdef DEBUG
        char temp_filename[256];
        qDebug()<<"output_page_" << QString(i + 1) << ".png" << '\n';  // save
        sprintf(temp_filename, "output_page_%d.png", i + 1);
        fz_save_pixmap_as_png(ctx,pix,temp_filename);
#endif
        // Convert fz_pixmap to FreeImage FIBITMAP
        FIBITMAP* bitmap = createFreeImageBitmap(pix);

        //resize to 128x128
        FIBITMAP* resized = resizeImageToThumbnail(bitmap, 128);
        if (!resized)
        {
            fprintf(stderr, "Cannot Resize image\n");
            FreeImage_Unload(bitmap);
            FreeImage_Unload(resized);
            fz_drop_pixmap(ctx,pix);
            return false;
        }

#define DEBUG
#ifdef DEBUG
        FreeImage_Save(FIF_PNG, resized, "output_filename.png", 0);
#endif
        if (!resized) {
            fprintf(stderr, "cannot convert pixmap to FreeImage bitmap\n");
            fz_drop_pixmap(ctx, pix);
            return false;
        }

        // Save the image data to a PNG file.
        char output_filename[256];
        sprintf(output_filename, "output_page_%d.png", i + 1);  // Create a unique filename for each page
        if (!FreeImage_Save(FIF_PNG, resized, output_filename, 0)) {
            fprintf(stderr, "cannot save image: %s\n", output_filename);
            FreeImage_Unload(resized);
            fz_drop_pixmap(ctx, pix);
            return false;
        }

        outputFiles.push_back(QString::fromStdString(output_filename));

        // Clean up.
        FreeImage_Unload(bitmap);
        FreeImage_Unload(resized);
        fz_drop_pixmap(ctx, pix);
    }

    return true;
}


// —оздание изображени€ FreeImage из fz_pixmap.
FIBITMAP* DocumentProcessor::createFreeImageBitmap(fz_pixmap* pixmap) {
    int width = fz_pixmap_width(ctx, pixmap);
    int height = fz_pixmap_height(ctx, pixmap);
    int n = fz_pixmap_components(ctx, pixmap);
    unsigned char* data = fz_pixmap_samples(ctx, pixmap);

    if (n != 3) {
        fprintf(stderr, "Unsupported color space\n");
        return nullptr;
    }

    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);// 24 бита на пиксель дл€ RGB
    if (!bitmap) {
        fprintf(stderr, "Failed to allocate FreeImage bitmap\n");
        return nullptr;
    }

    for (int y = 0; y < height; y++) {
        BYTE* bits = FreeImage_GetScanLine(bitmap, height - y - 1); // „итаем наоборот, тк изображени€ обычно хран€тс€ в формате "bottom-up"
        for (int x = 0; x < width; x++) {
            bits[FI_RGBA_RED] = data[y * width * n + x * n + 0];
            bits[FI_RGBA_GREEN] = data[y * width * n + x * n + 1];
            bits[FI_RGBA_BLUE] = data[y * width * n + x * n + 2];
            bits += 3;
        }
    }

    return bitmap;
}

// Изменение размера изображения FreeImage.
FIBITMAP* DocumentProcessor::resizeFreeImageBitmap(FIBITMAP* bitmap) {
    // Новые размеры для изменения размера.
    int newWidth = 128;
    int newHeight = 128;

    // Изменение размера изображения FreeImage
    FIBITMAP* resizedBitmap = FreeImage_Rescale(bitmap, newWidth, newHeight, FILTER_BICUBIC);

    return resizedBitmap;
}

FIBITMAP* DocumentProcessor::resizeImageToThumbnail(FIBITMAP* bitmap,int width) {
    int srcWidth = FreeImage_GetWidth(bitmap);
    int srcHeight = FreeImage_GetHeight(bitmap);

    // Determine new height based on aspect ratio
    int dstWidth = width;//128;
    int dstHeight = static_cast<int>(128.0 * srcHeight / srcWidth + 0.5);

    // Use FreeImage to resize the image
    FIBITMAP* dstBitmap = FreeImage_Rescale(bitmap, dstWidth, dstHeight, FILTER_BILINEAR);

    return dstBitmap;
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
