QT -= gui
QT += network
CONFIG += c++11 console
CONFIG -= app_bundle

INCLUDEPATH += "../libs/mupdf/thirdparty/curl/include"
INCLUDEPATH += ../libs/FreeImage/Dist
INCLUDEPATH += ../libs/mupdf/include
LIBS += -L../libs/mupdf/build/shared-release -lmupdf
LIBS += -lcurl
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../CurlUploader.cpp \
        ../DocumentProcessor.cpp \
        ../FileUploader.cpp \
        ../main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../CurlUploader.h \
    ../DocumentProcessor.h \
    ../FileUploader.h

unix|win32: LIBS += -lfreeimage-3.18.0

unix:!macx: LIBS += -lmupdf
