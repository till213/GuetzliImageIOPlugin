CONFIG += c++11

APP_NAME = SimpleGuetzliGUI

# Set the background color for images with alpha channel:
#
# 1: the image is blended against black (the original guetzli encoder does that - default)
# 2: the image is blended against white (the Yang for the above Yin)
# 3: the alpha channel is ignored (Qt does that e.g. when saving JPEG data)
#
# After modification "make clean" and recompile
DEFINES += GUETZLI_BLEND_MODE=1

# For unknown reasons to me FreeBSD 'make' trips over the mere existence
# of a subdirectory called 'obj'. Also refer to e.g.
# http://stackoverflow.com/questions/26002517/makefile-under-freebsd-does-not-compile-works-on-linux
# (using 'gmake' instead seems to solve the problem there, but does not
# explain why 'make' alone does not work)
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
} else {
    DESTDIR = $$PWD/bin/release
    message(Building $$TARGET in release mode)
}

macx {
    QMAKE_MAC_SDK = macosx10.13
}
