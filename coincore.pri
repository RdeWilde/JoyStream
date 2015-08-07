# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, July 29 2015

# Place in some global location
MSIGNA_DEPS_LOCATION = /home/bedeho/JoyStream/Development/libs/mSIGNA/deps

# Location of CoinCore folder in mSIGNA
COINCORE_LOCATION = $$MSIGNA_DEPS_LOCATION/CoinCore

# Windows
win32 {

}

# Unix
unix:!macx {

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
