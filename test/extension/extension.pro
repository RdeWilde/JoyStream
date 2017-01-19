include(gmock.pri)

TARGET = extension-test
TEMPLATE = app

CONFIG  += c++11

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../../extension/include

HEADERS += \
    include/MockPeer.hpp \
    include/MockTorrent.hpp \
    include/MockSession.hpp \
    include/MockAlertManager.hpp \

SOURCES += src/main.cpp

include(../../config.pri)
