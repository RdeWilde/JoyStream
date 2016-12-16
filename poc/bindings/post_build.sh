#!/bin/bash

spec=`qmake -query QMAKE_XSPEC`

DESTINATION="Frameworks/"

mkdir -p $DESTINATION

case "$spec" in
    macx-clang)
      cp -R `qmake -query QT_INSTALL_LIBS`/QtCore.framework ${DESTINATION}
    ;;
    linux-g++)
      cp `qmake -query QT_INSTALL_LIBS`/libQt5Core.so ${DESTINATION}
      chrpath -r \$ORIGIN/../../Frameworks build/Release/NativeExtension.node
    ;;
    *)
      exit 1
    ;;
esac
