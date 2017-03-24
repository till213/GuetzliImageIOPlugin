#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T08:18:38
#
#-------------------------------------------------

include(Sources.pri)


QT       += core gui

TARGET = GuetzliIOPlugin
TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = $$[QT_INSTALL_PLUGINS]/imageformats

INCLUDEPATH = $$PWD/../Guetzli/guetzli $$PWD/./Guetzli/guetzli/third_party/butteraugli

CONFIG(debug, debug|release) {
    OBJECTS_DIR    = obj/debug
    MOC_DIR        = GeneratedFiles/debug
    UI_DIR         = GeneratedFiles/debug
    INCLUDEPATH   += GeneratedFiles/debug
} else {
    OBJECTS_DIR    = obj/release
    MOC_DIR        = GeneratedFiles/release
    UI_DIR         = GeneratedFiles/release
    INCLUDEPATH   += GeneratedFiles/release
}

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../bin/debug -lGuetzli
} else {
    LIBS += -L$$PWD/../bin/release -lGuetzli
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

