include(Sources.pri)

TARGET = GuetzliLib
TEMPLATE = lib
CONFIG += staticlib c++11 console warn_off
QT -= core gui

INCLUDEPATH =  $$PWD/guetzli $$PWD/guetzli/third_party/butteraugli

CONFIG(debug, debug|release) {
    OBJECTS_DIR    = bin/debug
    MOC_DIR        = GeneratedFiles/debug
    UI_DIR         = GeneratedFiles/debug
    INCLUDEPATH   += GeneratedFiles/debug
} else {
    OBJECTS_DIR    = bin/release
    MOC_DIR        = GeneratedFiles/release
    UI_DIR         = GeneratedFiles/release
    INCLUDEPATH   += GeneratedFiles/release
}


CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../lib/debug
    message(Building $$TARGET in debug mode)

    QMAKE_CXXFLAGS_DEBUG = -MMD -MP -g
} else {
    DESTDIR = $$PWD/../lib/release
    message(Building $$TARGET in release mode)

    QMAKE_CXXFLAGS_RELEASE = -O3 -MMD -MP -g
}
