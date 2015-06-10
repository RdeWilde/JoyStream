TARGET = common
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT      += core

INCLUDEPATH += $$PWD/include/common # be able to include w.r.t root of this project

HEADERS += \
    include/common/BitCoinRepresentation.hpp \
    include/common/DataSizeRepresentation.hpp \
    include/common/Fiat.hpp \
    include/common/BitCoinDisplaySettings.hpp

SOURCES += \
    src/BitCoinRepresentation.cpp \
    src/DataSizeRepresentation.cpp \
    src/BitCoinDisplaySettings.cpp
