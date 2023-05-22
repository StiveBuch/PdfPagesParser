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
