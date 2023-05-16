#include <mupdf/fitz.h>
#include <FreeImage.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <pdf_file>\n", argv[0]);
        return 1;
    }

    fz_context* context = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (!context) {
        printf("Failed to create context\n");
        return 1;
    }

    fz_register_document_handlers(context);

    fz_document* doc = fz_open_document(context, argv[1]);
    if (!doc) {
        printf("Failed to open document\n");
        fz_drop_context(context);
        return 1;
    }

    int page_count = fz_count_pages(context, doc);
    for (int i = 0; i < page_count; ++i) {
        fz_page* page = fz_load_page(context, doc, i);
        fz_matrix transform;
        fz_pre_rotate(transform, 0);
        fz_pre_scale(transform, 1.0, 1.0);

        fz_rect bounds;
        fz_bound_page(context, page, bounds);
        fz_transform_rect(bounds, transform);

        fz_irect bbox;
        fz_round_rect(bbox, bounds);

        fz_pixmap* pixmap = fz_new_pixmap_from_page_contents(context, page, &transform, fz_device_rgb(context), 0);

        // TODO: Сделать ресайз изображения с использованием FreeImage
        // TODO: Сохранить изображение в файл или отправить на сервер

        fz_drop_pixmap(context, pixmap);
        fz_drop_page(context, page);
    }
    FIBITMAP* originalImage = FreeImage_Load(FIF_PNG, "your_image.png", PNG_DEFAULT);
    int originalWidth = FreeImage_GetWidth(originalImage);
    int originalHeight = FreeImage_GetHeight(originalImage);

    // Calculate new dimensions with preserved aspect ratio
    float resizeRatio = std::min(128.0f / originalWidth, 128.0f / originalHeight);
    int newWidth = static_cast<int>(originalWidth * resizeRatio);
    int newHeight = static_cast<int>(originalHeight * resizeRatio);

    FIBITMAP* resizedImage = FreeImage_Rescale(originalImage, newWidth, newHeight, FILTER_BICUBIC);

    // Save the resized image
    FreeImage_Save(FIF_PNG, resizedImage, "resized_image.png", PNG_DEFAULT);

    FreeImage_Unload(originalImage);
    FreeImage_Unload(resizedImage);

    fz_drop_document(context, doc);
    fz_drop_context(context);
    return 0;
}
