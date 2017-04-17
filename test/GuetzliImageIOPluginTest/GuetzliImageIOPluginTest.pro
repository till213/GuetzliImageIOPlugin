include (../../Common.pri)
include(Sources.pri)

QT += testlib
TEMPLATE = app
TARGET = GuetzliImageIOPluginTest
INCLUDEPATH += .
CONFIG += c++11
CONFIG -= app_bundle

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../../bin/debug
    message(Building $$TARGET in debug mode)
} else {
    DESTDIR = $$PWD/../../bin/release
    message(Building $$TARGET in release mode)
}


