# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Mokhtar Naamani <mokhtar.naamani@gmail.com>, December 1 2015

# compiled dependencies
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib

# openssl
INCLUDEPATH += /usr/include/openssl
LIBS += -L/usr/lib/x86_64-linux-gnu

# libtorrent
LIBTORRENT_LOCATION= $$PWD/deps/linux/dist

# mSIGNA
MSIGNA_SYSROOT = $$PWD/deps/linux/src/mSIGNA/sysroot
