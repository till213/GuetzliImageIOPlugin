include(../Common.pri)
include(Sources.pri)

QT += core gui

TARGET       = GuetzliImageIOPlugin
TEMPLATE     = lib
CONFIG      += plugin
INCLUDEPATH += $$PWD/../GuetzliLib/guetzli $$PWD/../GuetzliLib/guetzli/third_party/butteraugli

# Use the environment variable QT_DEBUG_PLUGINS to debug plugin loading
# e.g. export QT_DEBUG_PLUGINS=1
CONFIG(debug, debug|release) {
    LIBS   += -L$$PWD/../lib/debug -lGuetzliLib
    macos {
        DESTDIR = $$PWD/../bin/debug/$${APP_NAME}.app/Contents/plugins/imageformats
    } else {
        DESTDIR = $$PWD/../bin/debug/imageformats
    }
} else {
    LIBS   += -L$$PWD/../lib/release -lGuetzliLib
    macos {
        DESTDIR = $$PWD/../bin/release/$${APP_NAME}.app/Contents/plugins/imageformats
    } else {
        DESTDIR = $$PWD/../bin/release/imageformats
    }
}

# Installation
target.path = $$[QT_INSTALL_PLUGINS]/imageformats
INSTALLS += target

