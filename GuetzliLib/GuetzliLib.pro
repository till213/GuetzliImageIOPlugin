include(Sources.pri)

TARGET = GuetzliLib
TEMPLATE = lib
CONFIG += staticlib

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
} else {
    DESTDIR = $$PWD/../bin/release
    message(Building $$TARGET in release mode)
}
