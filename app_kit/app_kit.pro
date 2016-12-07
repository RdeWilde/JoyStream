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
    src/TransactionSendBuffer.cpp \
    src/SavedTorrents.cpp \
    src/SavedTorrentParameters.cpp \
    src/SavedSessionParameters.cpp \
    src/TorrentAdder.cpp \
    src/TorrentBuyer.cpp \
    src/TorrentSeller.cpp \
    src/Worker.cpp \
    src/TorrentObserver.cpp

HEADERS += include/app_kit/AppKit.hpp \
    include/app_kit/DataDirectory.hpp \
    include/app_kit/Settings.hpp \
    include/app_kit/HelperFunctions.hpp \
    include/app_kit/kit.hpp \
    include/app_kit/TransactionSendBuffer.hpp \
    include/app_kit/SavedTorrents.hpp \
    include/app_kit/SavedTorrentParameters.hpp \
    include/app_kit/SavedSessionParameters.hpp \
    include/app_kit/TorrentAdder.hpp \
    include/app_kit/AddTorrentRequest.hpp \
    include/app_kit/TorrentBuyer.hpp \
    include/app_kit/TorrentSeller.hpp \
    include/app_kit/Worker.hpp \
    include/app_kit/WorkerResult.hpp \
    include/app_kit/TorrentObserver.hpp

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

