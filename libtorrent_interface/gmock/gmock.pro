include(gmock.pri)

INCLUDEPATH += $$PWD/../interfaces/include
INCLUDEPATH += $$PWD/include

HEADERS += \
    include/MockPeer.hpp \
    include/MockTorrent.hpp \
    include/MockSession.hpp \

SOURCES += src/main.cpp

include(../../config.pri)
