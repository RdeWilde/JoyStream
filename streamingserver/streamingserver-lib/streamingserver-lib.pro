
TEMPLATE = lib
TARGET = streamingserver-lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html

QT += core network

INCLUDEPATH = include

HEADERS += \
    include/streamingserver/HttpServer.hpp \
    include/streamingserver/HttpRequestHandler.hpp \
    include/streamingserver/StreamingServer.hpp


SOURCES += \
    src/HttpRequestHandler.cpp \
    src/HttpServer.cpp \
    src/Streamingserver.cpp
