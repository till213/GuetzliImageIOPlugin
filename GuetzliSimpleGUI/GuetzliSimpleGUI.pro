#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T21:17:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GuetzliSimpleGUI
TEMPLATE = app

CONFIG(debug, debug|release) {
    OBJECTS_DIR    = object/debug
    MOC_DIR        = GeneratedFiles/debug
    UI_DIR         = GeneratedFiles/debug
    INCLUDEPATH   += GeneratedFiles/debug
} else {
    OBJECTS_DIR    = object/release
    MOC_DIR        = GeneratedFiles/release
    UI_DIR         = GeneratedFiles/release
    INCLUDEPATH   += GeneratedFiles/release
}

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/debug
    message(Building $$TARGET in debug mode)

    QMAKE_CXXFLAGS_DEBUG = -MMD -MP -g
} else {
    DESTDIR = $$PWD/bin/release
    message(Building $$TARGET in release mode)

    QMAKE_CXXFLAGS_RELEASE = -O3 -MMD -MP -g
}

SOURCES += main.cpp\
           MainWindow.cpp

HEADERS += MainWindow.h

FORMS   += MainWindow.ui
