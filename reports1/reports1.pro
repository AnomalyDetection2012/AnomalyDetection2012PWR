#-------------------------------------------------
#
# Project created by QtCreator 2012-11-24T21:14:55
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = reports1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ReportGenerator.cpp \
    WebViewPrinter.cpp

HEADERS  += mainwindow.h \
    ReportGenerator.h \
    WebViewPrinter.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
