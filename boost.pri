# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

#DEFINES += BOOST_ASIO_SEPARATE_COMPILATION
#DEFINES += BOOST_ASIO_HASH_MAP_BUCKETS=1021
#DEFINES += BOOST_ASIO_ENABLE_CANCELIO
#DEFINES += BOOST_EXCEPTION_DISABLE

# Took two days to find this: is required at least for boost 1.58.0
# =================================================================
# With out it, when in debug mode, you get the linker complaining because it cannot find
# * boost::asio::error::get_netdb_category()
# * boost::asio::error::get_addrinfo_category()
# * boost::asio::error::get_misc_category()
# the implementation is provided in boost/asio/impl/error.ipp,
# which is included in error.hpp conditional BOOST_ASIO_HEADER_ONLY macro being defined.
# Not sure how the code gets included in release mode, but it does
# <test if this is still needed>
#DEFINES += BOOST_ASIO_HEADER_ONLY

# Windows
win32 {
   CONFIG(release, debug|release) {
      BOOST_LIB_SUFFIX = -mt-s
   } else {
      BOOST_LIB_SUFFIX = -mt-sd
   }

    BOOST_THREAD_LIB_SUFFIX = _win32

    INCLUDEPATH += /usr/i686-w64-mingw32/include

    LIBS += \
        -L/usr/i686-w64-mingw32/lib \
        -L/usr/i686-w64-mingw32/plugins/platforms \
        -L/usr/i686-w64-mingw32/plugins/imageformats \
        -L/usr/i686-w64-mingw32/plugins/bearer \
        -L/usr/i686-w64-mingw32/plugins/sqldrivers \
        -static-libgcc -static-libstdc++ -static \
        -lpthread \
        -lws2_32 \
        -lmswsock

   LIBS += \
    -lboost_system$$BOOST_LIB_SUFFIX \
    -lboost_filesystem$$BOOST_LIB_SUFFIX \
    -lboost_regex$$BOOST_LIB_SUFFIX \
    -lboost_thread$$BOOST_THREAD_LIB_SUFFIX$$BOOST_LIB_SUFFIX \
    -lboost_serialization$$BOOST_LIB_SUFFIX \
    -lz
}

# Mac
macx {

    INCLUDEPATH += /usr/local/include

    LIBS += \
         -L/usr/local/lib \
         -lboost_thread \
         -lboost_system \
         -lboost_filesystem \
         -lboost_regex \
         -lpthread \
         -lz
}

# Unix
unix:!macx {

INCLUDEPATH += /usr/include

# SHARED
LIBS += \
        -L/usr/lib/x86_64-linux-gnu \
        -lboost_thread \
        -lboost_system \
        -lboost_chrono \
        -lboost_random \
        -lboost_filesystem \
        -lboost_regex \
        -lpthread \
        -lz

#LIBS += /usr/lib/x86_64-linux-gnu/ -lboost_thread
#LIBS += /usr/lib/x86_64-linux-gnu/ -lboost_system
#LIBS += /usr/lib/x86_64-linux-gnu/ -lboost_chrono
#LIBS += /usr/lib/x86_64-linux-gnu/ -lboost_filesystem
#LIBS += /usr/lib/x86_64-linux-gnu/ -lboost_regex
#LIBS += /usr/lib/x86_64-linux-gnu/ -lpthread
#LIBS += /usr/lib/x86_64-linux-gnu/ -lz
}
