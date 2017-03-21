#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T08:18:38
#
#-------------------------------------------------

include(Sources.pri)
include(GuetzliSources.pri)

QT       += core gui

TARGET = GuetzliIOPlugin
TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = $$[QT_INSTALL_PLUGINS]/imageformats

INCLUDEPATH = $$PWD/guetzli $$PWD/guetzli/third_party/butteraugli


unix {
    target.path = /usr/lib
    INSTALLS += target
}

