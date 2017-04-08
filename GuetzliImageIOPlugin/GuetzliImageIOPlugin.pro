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

INCLUDEPATH = $$PWD/../GuetzliLib/guetzli $$PWD/../GuetzliLib/guetzli/third_party/butteraugli

CONFIG(debug, debug|release) {
    OBJECTS_DIR    = object/debug
    MOC_DIR        = GeneratedFiles/debug
    UI_DIR         = GeneratedFiles/debug
    INCLUDEPATH   += GeneratedFiles/debug
} else {
    OBJECTS_DIR    = object/release
    MOC_DIR        = GeneratedFiles/release
    UI_DIR         = GeneratedFiles/release
    INCLUDEPATH   += GeneratedFiles/release
}

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../lib/debug -lGuetzliLib
} else {
    LIBS += -L$$PWD/../lib/release -lGuetzliLib
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

