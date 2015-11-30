export INSTALLPREFIX=/usr/i686-w64-mingw32
export OPENSSL_LIBS='-lssl -lcrypto -lgdi32'
cd qt5-win32-build-release
../qt-everywhere-opensource-src-5.5.0/configure -prefix / -hostprefix $INSTALLPREFIX/host -plugindir /plugins -translationdir /translations \
        -xplatform win32-g++ -device-option CROSS_COMPILE=i686-w64-mingw32- -sysroot $INSTALLPREFIX -opensource -static -qt-pcre \
        -no-icu -no-glib -no-nis -no-cups -no-iconv -no-dbus  -nomake examples -nomake tests \
        -no-feature-style-windowsce -no-feature-style-windowsmobile \
        -no-pch -no-compile-examples -no-qml-debug -no-sm \
        -skip qtwebkit -skip qtwebkit-examples -skip qtactiveqt -qt-zlib -openssl-linked -release

