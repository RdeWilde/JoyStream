#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T14:36:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = classic-gui
TEMPLATE = app
CONFIG  += c++11 # Needed for class enum, std::array

SOURCES += main.cpp\
    ClassicGUIDemoDialog.cpp

HEADERS  += \
    ClassicGUIDemoDialog.hpp

FORMS    += \
    ClassicGUIDemoDialog.ui

# gui
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../classic/gui/release/ -lgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../classic/gui/debug/ -lgui
else:unix: LIBS += -L$$OUT_PWD/../../classic/gui/ -lgui

INCLUDEPATH += $$PWD/../../classic/gui/include
DEPENDPATH += $$PWD/../../classic/gui/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../classic/gui/release/libgui.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../classic/gui/debug/libgui.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../classic/gui/release/gui.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../classic/gui/debug/gui.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../classic/gui/libgui.a

# common
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../common/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../common/debug/ -lcommon
else:unix: LIBS += -L$$OUT_PWD/../../common/ -lcommon

INCLUDEPATH += $$PWD/../../common/include
DEPENDPATH += $$PWD/../../common/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/libcommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/libcommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/release/common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../common/debug/common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../common/libcommon.a

include(../../config.pri)
