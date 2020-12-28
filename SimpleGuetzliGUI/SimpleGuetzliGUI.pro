include(../Common.pri)
include(Sources.pri)

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = $$APP_NAME
TEMPLATE = app

QMAKE_TARGET_COMPANY=till-art.net
QMAKE_TARGET_DESCRIPTION="Simple GUI for the guetzli encoder"
QMAKE_TARGET_PRODUCT="Simple Guetzli GUI"
VERSION=1.0.0

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
    QMAKE_INFO_PLIST = $$PWD/Info.plist

    OTHER_FILES += $$PWD/Info.plist \
                   $$PWD/res/GuetzliIcon.icns
}

OTHER_FILES += res/GuetzliIcon.xcf \
               res/GuetzliIcon.ico

RESOURCES += res/SimpleGuetzliGUIResources.qrc

version.target = src/generated_Version.cpp
message(Creating version...)
#TODO FIXME How to get a proper #include statement
version.commands = @echo \"$$LITERAL_HASH include \\\"Version.h\\\"\" > \"$$PWD/src/generated_Version.cpp\"
version.depends = FORCE

QMAKE_EXTRA_TARGETS += version
PRE_TARGETDEPS += src/generated_Version.cpp
