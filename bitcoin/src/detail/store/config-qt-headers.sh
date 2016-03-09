#!/bin/bash
mkdir -p ./qt_headers
ln -s `qmake -query QT_INSTALL_LIBS`/QtCore.framework/Headers/ ./qt_headers/QtCore
