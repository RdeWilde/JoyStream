QT       -= gui

TARGET = appkit
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

SOURCES += src/AppKit.cpp

HEADERS += include/appkit/AppKit.hpp

INCLUDEPATH += $$PWD/include

INCLUDEPATH += $$PWD/../common/include \
               $$PWD/../core/include \
               $$PWD/../extension/include \
               $$PWD/../protocol_session/include \
               $$PWD/../protocol_statemachine/include \
               $$PWD/../protocol_wire/include \
               $$PWD/../paymentchannel/include \
               $$PWD/../bitcoin/include

include(../config.pri)