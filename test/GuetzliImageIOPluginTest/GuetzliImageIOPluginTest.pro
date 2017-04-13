include(Sources.pri)

QT += testlib
TEMPLATE = app
TARGET = GuetzliImageIOPluginTest
INCLUDEPATH += .
CONFIG += c++11
CONFIG -= app_bundle

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
    DESTDIR = $$PWD/../../bin/debug
    message(Building $$TARGET in debug mode)
} else {
    DESTDIR = $$PWD/../../bin/release
    message(Building $$TARGET in release mode)
}

