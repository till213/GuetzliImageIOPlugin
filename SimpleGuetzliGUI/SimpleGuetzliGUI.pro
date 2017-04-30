include(../Common.pri)
include(Sources.pri)

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = $$APP_NAME
TEMPLATE = app

QMAKE_TARGET_COMPANY=till-art.net
QMAKE_TARGET_DESCRIPTION="Simple GUI for the guetzli encoder"
QMAKE_TARGET_PRODUCT="Simple GuetzliGUI"
VERSION=0.1

# Application icon
win32 {
    RC_ICONS = res/GuetzliIcon.ico
}
macx {
    iconset.target = iconset
    iconset.commands = iconutil -c icns $$PWD/res/GuetzliIcon.iconset

    QMAKE_EXTRA_TARGETS += iconset
    PRE_TARGETDEPS += iconset
    ICON = $$PWD/res/GuetzliIcon.icns
}

OTHER_FILES += res/GuetzliIcon.xcf \
               res/GuetzliIcon.ico


