# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

DEFINES += BOOST_ASIO_SEPARATE_COMPILATION
DEFINES += BOOST_ASIO_HASH_MAP_BUCKETS=1021
DEFINES += BOOST_ASIO_ENABLE_CANCELIO
DEFINES += BOOST_EXCEPTION_DISABLE

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
DEFINES += BOOST_ASIO_HEADER_ONLY

# Windows
win32 {

    BOOST_LOCATION = C:/boost_1_57_0

    INCLUDEPATH += $$BOOST_LOCATION
    LIBS += -L$$BOOST_LOCATION/stage/lib

}

# Mac

# Unix
unix:!macx {

    INCLUDEPATH += /usr/local/include

    LIBS += \
         -L/usr/local/lib \
         -lboost_thread \
         -lboost_system \
         -lboost_filesystem \
         -lboost_regex \
         -lpthread \
         -lz

#    CONFIG(release, debug|release) {
#
#        INCLUDEPATH += /usr/local/include
#
#        LIBS += \
#             -L/usr/local/lib \
#             -lboost_thread \
#             -lboost_system \
#             -lboost_filesystem \
#             -lboost_regex \
#             -lpthread \
#             -lz
#
#    } else {
#
#       # only bullshit way I found to link with debug build
#       BOOST_BUILD_LOCATION = /home/bedeho/build_boost/boost/bin.v2/libs
#
#       INCLUDEPATH += /usr/local/include
#
#       LIBS += -L$$BOOST_BUILD_LOCATION/thread/build/gcc-4.8/debug/link-static/threading-multi \
#               -lboost_thread
#
#       LIBS += -L$$BOOST_BUILD_LOCATION/system/build/gcc-4.8/debug/link-static/threading-multi \
#              -lboost_system
#
#       LIBS += -L$$BOOST_BUILD_LOCATION/filesystem/build/gcc-4.8/debug/link-static/threading-multi \
#              -lboost_filesystem
#
#    }

}
