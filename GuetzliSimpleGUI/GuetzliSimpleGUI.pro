include(../Common.pri)
include(Sources.pri)

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = $$APP_NAME
TEMPLATE = app

# Application icon
win32 {
    RC_ICONS = res/GuetzliIcon.ico
}



