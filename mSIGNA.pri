# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 7 2015

##########################
# mSIGNA
##########################

# Windows
win32 {

}

# Unix
unix:!macx {

    MSIGNA_SYSROOT = /home/bedeho/JoyStream/Development/libs/mSIGNA/sysroot

    INCLUDEPATH += $$MSIGNA_SYSROOT/include

    LIBS += -L$$MSIGNA_SYSROOT/lib \
            #-lCoinDB \
            #-lCoinQ \
            -lCoinCore \
            -llogger \
            -lsysutils
}
