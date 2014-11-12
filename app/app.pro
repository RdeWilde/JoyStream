include(../defaults.pri)

TARGET = BitSwapr
TEMPLATE = app
CONFIG  += console
SOURCES += main.cpp

win32:CONFIG(release, debug|release):       LIBS += -L$$OUT_PWD/../lib/release/ -lQtBitSwapr
else:win32:CONFIG(debug, debug|release):    LIBS += -L$$OUT_PWD/../lib/debug/ -lQtBitSwapr
else:unix:                                  LIBS += -L$$OUT_PWD/../lib/ -lQtBitSwapr

DEPENDPATH += $$PWD/../lib
