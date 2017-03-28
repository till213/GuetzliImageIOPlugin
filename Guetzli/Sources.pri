DEPENDPATH += $$PWD/GeneratedFiles \
              $$PWD/src

HEADERS   +=
SOURCES   += $$PWD/../GuetzliLib/guetzli/guetzli/guetzli.cc

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../bin/debug -lGuetzliLib
} else {
    LIBS += -L$$PWD/../bin/release -lGuetzliLib
}


