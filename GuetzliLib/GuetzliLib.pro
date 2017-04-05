include(Sources.pri)

TARGET = GuetzliLib
TEMPLATE = lib
CONFIG += staticlib c++11 console warn_off
QT -= core gui

INCLUDEPATH =  $$PWD/guetzli $$PWD/guetzli/third_party/butteraugli

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
    DESTDIR = $$PWD/../bin/debug
    message(Building $$TARGET in debug mode)

    QMAKE_CXXFLAGS = -stdlib=libc++ -MMD -MP -g -std=c++11
} else {
    DESTDIR = $$PWD/../bin/release
    message(Building $$TARGET in release mode)

    QMAKE_CXXFLAGS_RELEASE = -O3
    QMAKE_CXXFLAGS = -stdlib=libc++ -MMD -MP -g -std=c++11
}
