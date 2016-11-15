QT += core
QT += widgets
QT -= gui

CONFIG += c++11

TARGET = joystreamd
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../app_kit/release/ -lapp_kit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../app_kit/debug/ -lapp_kit
else:unix: LIBS += -L$$OUT_PWD/../app_kit/ -lapp_kit

INCLUDEPATH += $$PWD/../app_kit/include
DEPENDPATH += $$PWD/../app_kit

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app_kit/release/libapp_kit.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app_kit/debug/libapp_kit.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app_kit/release/app_kit.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app_kit/debug/app_kit.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../app_kit/libapp_kit.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core/include
DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bitcoin/release/ -lbitcoin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bitcoin/debug/ -lbitcoin
else:unix: LIBS += -L$$OUT_PWD/../bitcoin/ -lbitcoin

INCLUDEPATH += $$PWD/../bitcoin/include
DEPENDPATH += $$PWD/../bitcoin

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bitcoin/release/libbitcoin.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bitcoin/debug/libbitcoin.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bitcoin/release/bitcoin.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bitcoin/debug/bitcoin.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../bitcoin/libbitcoin.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../extension/release/ -lextension
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../extension/debug/ -lextension
else:unix: LIBS += -L$$OUT_PWD/../extension/ -lextension

INCLUDEPATH += $$PWD/../extension/include
DEPENDPATH += $$PWD/../extension

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../extension/release/libextension.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../extension/debug/libextension.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../extension/release/extension.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../extension/debug/extension.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../extension/libextension.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../protocol_wire/release/ -lprotocol_wire
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../protocol_wire/debug/ -lprotocol_wire
else:unix: LIBS += -L$$OUT_PWD/../protocol_wire/ -lprotocol_wire

INCLUDEPATH += $$PWD/../protocol_wire/include
DEPENDPATH += $$PWD/../protocol_wire

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_wire/release/libprotocol_wire.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_wire/debug/libprotocol_wire.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_wire/release/protocol_wire.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_wire/debug/protocol_wire.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../protocol_wire/libprotocol_wire.a


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../protocol_session/release/ -lprotocol_session
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../protocol_session/debug/ -lprotocol_session
else:unix: LIBS += -L$$OUT_PWD/../protocol_session/ -lprotocol_session

INCLUDEPATH += $$PWD/../protocol_session/include
DEPENDPATH += $$PWD/../protocol_session

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_session/release/libprotocol_session.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_session/debug/libprotocol_session.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_session/release/protocol_session.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_session/debug/protocol_session.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../protocol_session/libprotocol_session.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../protocol_statemachine/release/ -lprotocol_statemachine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../protocol_statemachine/debug/ -lprotocol_statemachine
else:unix: LIBS += -L$$OUT_PWD/../protocol_statemachine/ -lprotocol_statemachine

INCLUDEPATH += $$PWD/../protocol_statemachine/include
DEPENDPATH += $$PWD/../protocol_statemachine

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_statemachine/release/libprotocol_statemachine.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_statemachine/debug/libprotocol_statemachine.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_statemachine/release/protocol_statemachine.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protocol_statemachine/debug/protocol_statemachine.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../protocol_statemachine/libprotocol_statemachine.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../paymentchannel/release/ -lpaymentchannel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../paymentchannel/debug/ -lpaymentchannel
else:unix: LIBS += -L$$OUT_PWD/../paymentchannel/ -lpaymentchannel

INCLUDEPATH += $$PWD/../paymentchannel/include
DEPENDPATH += $$PWD/../paymentchannel

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/release/libpaymentchannel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/debug/libpaymentchannel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/release/paymentchannel.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/debug/paymentchannel.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../paymentchannel/libpaymentchannel.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../common/ -lcommon

INCLUDEPATH += $$PWD/../common/include
DEPENDPATH += $$PWD/../common

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../common/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../common/libcommon.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../joystreamd_lib/release/ -ljoystreamd_lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../joystreamd_lib/debug/ -ljoystreamd_lib
else:unix: LIBS += -L$$OUT_PWD/../joystreamd_lib/ -ljoystreamd_lib

INCLUDEPATH += $$PWD/../joystreamd_lib/include
INCLUDEPATH += $$PWD/../joystreamd_lib
DEPENDPATH += $$PWD/../joystreamd_lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../joystreamd_lib/release/libjoystreamd_lib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../joystreamd_lib/debug/libjoystreamd_lib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../joystreamd_lib/release/joystreamd_lib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../joystreamd_lib/debug/joystreamd_lib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../joystreamd_lib/libjoystreamd_lib.a

# gRPC and protocol buffers libs
LIBS += -lgrpc++ -lgrpc -lgpr -lgrpc++_reflection -lprotobuf

include(../config.pri)
