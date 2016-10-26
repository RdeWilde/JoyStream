QT       -= gui

TARGET = app_kit
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += c++11 # Needed for class enum

SOURCES += src/AppKit.cpp \
    src/DataDirectory.cpp \
    src/Settings.cpp \
    src/HelperFunctions.cpp \
    src/TransactionSendQueue.cpp

HEADERS += include/app_kit/AppKit.hpp \
    include/app_kit/DataDirectory.hpp \
    include/app_kit/Settings.hpp \
    include/app_kit/HelperFunctions.hpp \
    include/app_kit/kit.hpp \
    include/app_kit/TransactionSendQueue.hpp

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

