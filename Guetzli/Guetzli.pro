#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T21:17:49
#
#-------------------------------------------------

include(Sources.pri)

QT       +=

TARGET = guetzli
TEMPLATE = app

INCLUDEPATH = $$PWD/../GuetzliLib/guetzli/guetzli $$PWD/../GuetzliLib/guetzli/third_party/butteraugli

# This is a hack (for macOS) which presumably breaks in Qt 5.8 (latest):
# The binary Qt distribution for macOS does not have PKGCONFIG support build in
QT_CONFIG -= no-pkg-config

CONFIG += link_pkgconfig
PKGCONFIG += png
