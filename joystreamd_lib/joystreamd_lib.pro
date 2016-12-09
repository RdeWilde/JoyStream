# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential

QT += widgets #Required for QApplication
QT -= gui

TARGET = joystreamd_lib
TEMPLATE = lib
CONFIG  += staticlib
CONFIG  += c++11 # Needed for class enum

INCLUDEPATH += $$PWD/include

INCLUDEPATH += $$PWD/../common/include \
               $$PWD/../core/include \
               $$PWD/../extension/include \
               $$PWD/../protocol_session/include \
               $$PWD/../protocol_statemachine/include \
               $$PWD/../protocol_wire/include \
               $$PWD/../paymentchannel/include \
               $$PWD/../bitcoin/include \
               $$PWD/../app_kit/include

SOURCES += \
    src/RPCPause.cpp \
    src/ServerImpl.cpp \
    src/CompletionQueueDispatcher.cpp \
    src/RPCAddTorrent.cpp \
    src/RPCRemoveTorrent.cpp \
    src/RPCRequest.cpp \
    src/RPCListTorrents.cpp \
    src/RPCPauseTorrent.cpp \
    src/RPCStartTorrent.cpp \
    protos/daemon.grpc.pb.cc \
    protos/daemon.pb.cc \
    src/RPCReceivedAddress.cpp \
    src/RPCBalance.cpp \
    src/RPCUnconfirmedBalance.cpp \
    src/RPCStatus.cpp \
    src/RPCSuscribeStatus.cpp \
    src/RPCBuyTorrent.cpp \
    src/RPCSuscribeEvents.cpp \
    src/RPCGetTorrentState.cpp \
    src/RPCSellTorrent.cpp \
    src/RPCSyncWallet.cpp

HEADERS += \
    include/joystreamd_lib/RPCRequestNormal.hpp \
    include/joystreamd_lib/RPCRequestStreamServer.hpp \
    include/joystreamd_lib/RPCPause.hpp \
    include/joystreamd_lib/ServerImpl.hpp \
    include/joystreamd_lib/RPCRequest.hpp \
    include/joystreamd_lib/CompletionQueueDispatcher.hpp \
    include/joystreamd_lib/RPCAddTorrent.hpp \
    include/joystreamd_lib/RPCRemoveTorrent.hpp \
    include/joystreamd_lib/RPCListTorrents.hpp \
    include/joystreamd_lib/RPCPauseTorrent.hpp \
    include/joystreamd_lib/RPCStartTorrent.hpp \
    include/joystreamd_lib/RPCReceivedAddress.hpp \
    protos/daemon.grpc.pb.h \
    protos/daemon.pb.h \
    include/joystreamd_lib/RPCBalance.hpp \
    include/joystreamd_lib/RPCUnconfirmedBalance.hpp \
    include/joystreamd_lib/RPCStatus.hpp \
    include/joystreamd_lib/RPCSuscribeStatus.hpp \
    include/joystreamd_lib/RPCBuyTorrent.hpp \
    include/joystreamd_lib/RPCSuscribeEvents.hpp \
    include/joystreamd_lib/RPCGetTorrentState.hpp \
    include/joystreamd_lib/RPCSellTorrent.hpp \
    include/joystreamd_lib/RPCSyncWallet.hpp

# gRPC and protocol buffers libs
LIBS += -lgrpc++ -lgrpc -lgpr -lgrpc++_reflection -lprotobuf

include(../config.pri)
