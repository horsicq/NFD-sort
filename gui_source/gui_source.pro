QT       += core gui sql concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nfd-sort
TEMPLATE = app

SOURCES += \
        guimainwindow.cpp \
        main_gui.cpp \
        dialogscanprogress.cpp \
        scanprogress.cpp

HEADERS += \
        guimainwindow.h \
        ../global.h \
        dialogscanprogress.h \
        scanprogress.h

FORMS += \
        guimainwindow.ui \
        dialogscanprogress.ui

!contains(XCONFIG, staticscan) {
    XCONFIG += staticscan
    include(../../_mylibs/SpecAbstract/staticscan.pri)
}

include(../build.pri)

win32 {
    RC_ICONS = ../icons/main.ico
}

macx {
    ICON = ../icons/main.icns
}
