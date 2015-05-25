include (../../boost.pri)

TEMPLATE = app
TARGET = streamingserver-test

# I get warning "Project WARNING: CONFIG+=qtestlib is deprecated. Use QT+=testlib instead.",
# however,  if I actuallyuse testlib, then I get a link error indicating that testing library could
# not be linked with
CONFIG += qtestlib

CONFIG += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html

QT += core network

SOURCES += \
    main.cpp \
    TestStream.cpp

HEADERS += \
    TestStream.hpp

#INCLUDEPATH += $$PWD # be able to include w.r.t root of this project

# streamingserver-lib ###############################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../streamingserver-lib/release/ -lstreamingserver-lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../streamingserver-lib/debug/ -lstreamingserver-lib
else:unix: LIBS += -L$$OUT_PWD/../streamingserver-lib/ -lstreamingserver-lib

INCLUDEPATH += $$PWD/../streamingserver-lib/include
DEPENDPATH += $$PWD/../streamingserver-lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../streamingserver-lib/release/libstreamingserver-lib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../streamingserver-lib/debug/libstreamingserver-lib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../streamingserver-lib/release/streamingserver-lib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../streamingserver-lib/debug/streamingserver-lib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../streamingserver-lib/libstreamingserver-lib.a

