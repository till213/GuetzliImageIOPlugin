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

CONFIG += link_pkgconfig
PKGCONFIG += png
