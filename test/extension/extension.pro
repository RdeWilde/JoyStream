include(gmock.pri)

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../../extension/include
INCLUDEPATH += $$PWD/../../extension/interface

HEADERS += \
    include/MockPeer.hpp \
    include/MockTorrent.hpp \
    include/MockSession.hpp \
    include/MockAlertManager.hpp \

SOURCES += src/main.cpp

include(../../config.pri)
