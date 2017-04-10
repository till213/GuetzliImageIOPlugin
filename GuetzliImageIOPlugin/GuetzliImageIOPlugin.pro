#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T08:18:38
#
#-------------------------------------------------

include(Sources.pri)


QT += core gui

TARGET = GuetzliIOPlugin
TEMPLATE = lib
CONFIG += plugin c++11

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
    DESTDIR = $$PWD/../bin/debug/imageformats
} else {
    LIBS += -L$$PWD/../lib/release -lGuetzliLib
    DESTDIR = $$PWD/../bin/release/imageformats
}

unix {
    target.path = $$[QT_INSTALL_PLUGINS]/imageformats
    INSTALLS += target
}

