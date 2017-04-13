DEPENDPATH += $$PWD/GeneratedFiles \
              $$PWD/src \
              $$PWD/../GuetzliLib/guetzli \
              $$PWD/../GuetzliLib/guetzli/third_party/butteraugli

HEADERS   += $$PWD/src/GuetzliImageIOPlugin.h \
             $$PWD/src/GuetzliImageIOHandler.h
SOURCES   += $$PWD/src/GuetzliImageIOPlugin.cpp \
             $$PWD/src/GuetzliImageIOHandler.cpp
DISTFILES += $$PWD/src/GuetzliIOPlugin.json

