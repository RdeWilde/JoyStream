include(../tests_defaults.pri)

# Qt Project
TARGET = TestLoggerManager
TEMPLATE = app

CONFIG += console
CONFIG += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG += qtestlib
QT      += core

INCLUDEPATH += $$PWD # be able to include w.r.t root of this project

SOURCES += \
    TestLoggerManager.cpp

HEADERS += \
    TestLoggerManager.hpp

# Required for including libtorrent and boost headers
include(../../defaults.pri)
