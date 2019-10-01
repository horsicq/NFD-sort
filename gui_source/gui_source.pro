QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nfd-sort
TEMPLATE = app

SOURCES += \
        guimainwindow.cpp \
        main_gui.cpp

HEADERS += \
        guimainwindow.h \
    ../global.h

FORMS += \
        guimainwindow.ui

!contains(XCONFIG, formresult) {
    XCONFIG += formresult
    include(../StaticScan/formresult.pri)
}

include(../build.pri)

win32 {
    RC_ICONS = ../icons/main.ico
}

macx {
    ICON = ../icons/main.icns
}
