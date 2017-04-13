include(../Common.pri)
include(Sources.pri)

QT += core gui

TARGET       = GuetzliImageIOPlugin
TEMPLATE     = lib
CONFIG      += plugin
INCLUDEPATH += $$PWD/../GuetzliLib/guetzli $$PWD/../GuetzliLib/guetzli/third_party/butteraugli

CONFIG(debug, debug|release) {
    LIBS   += -L$$PWD/../lib/debug -lGuetzliLib
    DESTDIR = $$PWD/../bin/debug/imageformats
} else {
    LIBS   += -L$$PWD/../lib/release -lGuetzliLib
    DESTDIR = $$PWD/../bin/release/imageformats
}

# Installation
target.path = $$[QT_INSTALL_PLUGINS]/imageformats
INSTALLS += target

