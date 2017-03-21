#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T08:18:38
#
#-------------------------------------------------

include(Sources.pri)

QT       += core gui

TARGET = GuetzliIOPlugin
TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = $$[QT_INSTALL_PLUGINS]/imageformats



unix {
    target.path = /usr/lib
    INSTALLS += target
}

