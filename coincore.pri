# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, July 29 2015

# Windows
win32 {

}

# Unix
unix:!macx {

    MSIGNA_LOCATION = /home/bedeho/JoyStream/Development/libs/mSIGNA/sysroot

    INCLUDEPATH += $$COINCORE_LOCATION/src

    # Linking
    CONFIG(release, debug|release) {
        LIBS += -L$$COINCORE_LOCATION/lib -lCoinCore
    } else {
        LIBS += -L$$COINCORE_LOCATION/lib -lCoinCore
    }
}

# stdutils
INCLUDEPATH += $$MSIGNA_DEPS_LOCATION
