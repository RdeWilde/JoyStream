
# Qt Project
TARGET = BitSwapr
TEMPLATE = app

CONFIG  += console
CONFIG  += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
QT      += core network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # Needed for including QApplication

INCLUDEPATH += $$PWD # be able to include w.r.t root of this project
INCLUDEPATH += $$PWD/.. # be able to include w.r.t root of project, in particular content from lib sub project

SOURCES += main.cpp

# Required for including libtorrent and boost headers
include(../defaults.pri)

# Link with lib
# conditions required because on windows the builds are put in extra release/debug subfolders
win32:CONFIG(release, debug|release):       LIBS += -L$$OUT_PWD/../lib/release/ -lQtBitSwapr
else:win32:CONFIG(debug, debug|release):    LIBS += -L$$OUT_PWD/../lib/debug/ -lQtBitSwapr
else:unix:                                  LIBS += -L$$OUT_PWD/../lib/ -lQtBitSwapr
