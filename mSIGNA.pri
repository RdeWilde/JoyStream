# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 7 2015

# Windows
win32 {

}

# Unix
unix:!macx {

    CONFIG(release, debug|release) {
        MSIGNA_SYSROOT = /home/bedeho/JoyStream/Development/libs/mSIGNA-release/sysroot
    } else {
        MSIGNA_SYSROOT = /home/bedeho/JoyStream/Development/libs/mSIGNA-debug/sysroot
    }

    INCLUDEPATH += $$MSIGNA_SYSROOT/include

    LIBS += -L$$MSIGNA_SYSROOT/lib \
            -lCoinCore \
            -lCoinDB \
            -lCoinQ \
            -llogger \
            -lsysutils
}
