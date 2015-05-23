
# Required for including libtorrent and boost headers
include(../defaults.pri)

TARGET = tests
TEMPLATE = app

# I get warning "Project WARNING: CONFIG+=qtestlib is deprecated. Use QT+=testlib instead.",
# however,  if I actuallyuse testlib, then I get a link error indicating that testing library could
# not be linked with
CONFIG += qtestlib
CONFIG += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
QT     += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # Needed for including QApplication

INCLUDEPATH += $$PWD # be able to include w.r.t root of this project

SOURCES += \
    TestLoggerManager.cpp \
    TestTxId.cpp \
    main.cpp

HEADERS += \
    TestLoggerManager.hpp \
    TestTxId.hpp

# lib ###############################################################
win32:CONFIG(release, debug|release):       LIBS += -L$$OUT_PWD/../lib/release/ -lQtBitSwapr
else:win32:CONFIG(debug, debug|release):    LIBS += -L$$OUT_PWD/../lib/debug/ -lQtBitSwapr
else:unix:                                  LIBS += -L$$OUT_PWD/../lib/ -lQtBitSwapr

INCLUDEPATH += $$PWD/../lib
DEPENDPATH += $$PWD/../lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/release/libQtBitSwapr.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/debug/libQtBitSwapr.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/release/QtBitSwapr.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/debug/QtBitSwapr.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../lib/libQtBitSwapr.a
