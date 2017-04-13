include(../Common.pri)
include(Sources.pri)

QT += core gui

TARGET       = GuetzliImageIOPlugin
TEMPLATE     = lib
CONFIG      += plugin
INCLUDEPATH += $$PWD/../GuetzliLib/guetzli $$PWD/../GuetzliLib/guetzli/third_party/butteraugli

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../lib/debug -lGuetzliLib
} else {
    LIBS += -L$$PWD/../lib/release -lGuetzliLib
}

# Installation
target.path = $$[QT_INSTALL_PLUGINS]/imageformats
INSTALLS += target

