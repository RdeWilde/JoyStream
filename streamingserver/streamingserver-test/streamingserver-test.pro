
TEMPLATE = app
TARGET = streamingserver-test

# I get warning "Project WARNING: CONFIG+=qtestlib is deprecated. Use QT+=testlib instead.",
# however,  if I actuallyuse testlib, then I get a link error indicating that testing library could
# not be linked with
CONFIG += qtestlib

CONFIG += link_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
QT += core network

SOURCES += \
    main.cpp
