include(gmock.pri)

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/MockPeer.hpp \
    include/MockTorrent.hpp \
    include/MockSession.hpp \
    include/MockAlertManager.hpp \

SOURCES += src/main.cpp

include(../../config.pri)
