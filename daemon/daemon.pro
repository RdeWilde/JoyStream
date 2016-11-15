# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential

QT += widgets #Required for QApplication
QT -= gui

TARGET = daemon
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
               $$PWD/../bitcoin/include

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
    protos/daemon.pb.cc

HEADERS += \
    include/daemon/RPCRequestNormal.hpp \
    include/daemon/RPCRequestStreamServer.hpp \
    include/daemon/RPCPause.hpp \
    include/daemon/ServerImpl.hpp \
    include/daemon/RPCRequest.hpp \
    include/daemon/CompletionQueueDispatcher.hpp \
    include/daemon/RPCAddTorrent.hpp \
    include/daemon/RPCRemoveTorrent.hpp \
    include/daemon/RPCListTorrents.hpp \
    include/daemon/RPCPauseTorrent.hpp \
    include/daemon/RPCStartTorrent.hpp \
    protos/daemon.grpc.pb.h \
    protos/daemon.pb.h

# gRPC and protocol buffers libs
LIBS += -lgrpc++ -lgrpc -lgpr -lgrpc++_reflection -lprotobuf

include(../config.pri)
