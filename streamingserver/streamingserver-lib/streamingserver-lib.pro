
TEMPLATE = lib
TARGET = streamingserver-lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

QT += core network

INCLUDEPATH = include

HEADERS += \
    include/streamingserver/HttpServer.hpp \
    include/streamingserver/HttpConnectionHandler.hpp

SOURCES += \
    src/HttpServer.cpp \
    src/HttpConnectionHandler.cpp
