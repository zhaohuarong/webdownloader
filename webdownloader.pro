QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = webdownloader
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/core

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    core/downloader.cpp \
    core/accessmanager.cpp

HEADERS += \
        mainwindow.h \
    core/downloader.h \
    core/accessmanager.h

FORMS += \
        mainwindow.ui
