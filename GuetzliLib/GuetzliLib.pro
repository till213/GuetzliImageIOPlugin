include (../Common.pri)
include(Sources.pri)

TARGET   = GuetzliLib
TEMPLATE = lib
CONFIG  += staticlib console warn_off
QT      -= core gui

INCLUDEPATH =  $$PWD/guetzli $$PWD/guetzli/third_party/butteraugli

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../lib/debug
    QMAKE_CXXFLAGS_DEBUG = -MMD -MP -g
} else {
    DESTDIR = $$PWD/../lib/release
    QMAKE_CXXFLAGS_RELEASE = -O3 -MMD -MP -g
}
