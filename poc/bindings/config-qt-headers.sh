#!/bin/bash
rm -fr ./qt_headers

#determine which target platform we are building for
spec=`qmake -query QMAKE_XSPEC`

case "$spec" in
    win32-g++)
	mkdir ./qt_headers
	ln -s `qmake -query QT_INSTALL_HEADERS`/QtCore ./qt_headers/QtCore
    ;;
    macx-clang)
	mkdir ./qt_headers
	ln -s `qmake -query QT_INSTALL_LIBS`/QtCore.framework/Headers/ ./qt_headers/QtCore
    ;;
    linux-g++)
	mkdir ./qt_headers
	ln -s `qmake -query QT_INSTALL_HEADERS`/QtCore ./qt_headers/QtCore
    ;;
    *)
        echo "I don't know how to build for " $spec
        exit 1
    ;;
esac
