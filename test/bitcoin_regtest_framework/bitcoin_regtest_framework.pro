#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T12:03:22
#
#-------------------------------------------------

QT       -= gui

TARGET = bitcoin_regtest_framework
TEMPLATE = lib

CONFIG  += c++11
CONFIG  -= core

DEFINES += BITCOIN_REGTEST_FRAMEWORK_LIBRARY

SOURCES += bitcoin_regtest_framework.cpp

HEADERS += bitcoin_regtest_framework.hpp\
        bitcoin_regtest_framework_global.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
