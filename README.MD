# PdfPagesParser

## Description
This software is developed in C++ and utilizes several libraries and tools, including muPdf, FreeImage, Qt, QtNetwork, and libcurl. The main objective of the program is to create thumbnails of PDF pages and upload them to a Flask server.

## Components
Let's delve into each of the components used in more detail:

muPdf: It is a library that provides powerful tools for working with PDF files. It enables extracting information from PDF documents and generating page images.

FreeImage: This is a freely available library that offers a wide range of functions for working with images. In this case, it is used for processing and creating thumbnail images from PDF pages.

Qt: It is a cross-platform framework for developing graphical user interfaces. Qt offers a rich set of tools for image manipulation, file operations, and network communication, making it an ideal choice for this project.

QtNetwork: It is a Qt module that provides classes and functions for network operations. It allows for sending and receiving data over HTTP protocols, which will be used for uploading the generated thumbnail images to the Flask server.

libcurl: It is a library for transferring data over various protocols, including HTTP. It provides low-level access to network operations and will be used in conjunction with QtNetwork to implement the uploading of thumbnails to the server.

## Functionality
The main functionality of the program includes the following steps:

Reading the PDF file using the muPdf library to extract individual pages.

Utilizing FreeImage to create thumbnail images for each PDF page. This involves processing and resizing the images.

Employing QtNetwork and libcurl to upload the generated thumbnail images to the Flask server. This includes constructing HTTP requests and transmitting the images as data.

In summary, this program enables users to select a PDF file, generate thumbnails for each page using muPdf and FreeImage, and then upload the generated thumbnails to a Flask server using Qt and libcurl. This can be useful, for example, for creating a gallery or image archive from PDF files on a web server.

Regarding deploying and building each library separately on Linux, here's a brief guide:

## Dependencies

muPdf: You can download the source code from the muPdf website (https://mupdf.com/downloads/). Follow the provided instructions for building and installing the library on Linux. This typically involves running the configure script and then executing the make and make install commands.

FreeImage: Visit the FreeImage website (http://freeimage.sourceforge.net/download.html) to download the source code. Once downloaded, extract the files and navigate to the extracted directory. Use the provided makefile or build script to compile the library. After successful compilation, you can install it using the make install command.

Qt: The Qt framework offers precompiled binaries for various Linux distributions. You can download the installer from the Qt website (https://www.qt.io/download). Follow the installation instructions specific to your Linux distribution. Alternatively, you can also build Qt from source by downloading the source code and following the instructions provided in the Qt documentation.

QtNetwork and libcurl: QtNetwork is part of the Qt framework, so it will be installed when you install Qt. For libcurl, you can install it using your Linux distribution's package manager. For example, on Ubuntu, you can use the following command: 
```bash
sudo apt-get install libcurl4-openssl-dev
```

Make sure to link the necessary libraries when compiling your C++ program. You can specify the library paths and include directories using the appropriate compiler flags. For example, using g++ compiler: `g++

## Build and install

To install and use the muPdf library on Debian, you can follow these steps:
```bash
sudo apt-get install build-essential libfreetype6-dev libjpeg-dev libjbig2dec0-dev zlib1g-dev libopenjpeg-dev libfontconfig1-dev libx11-dev libxt-dev
```
Build the muPdf library:
```bash
make
```
Once the build process completes successfully, you can install the library:
```bash
sudo make install
```