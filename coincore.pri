# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, July 29 2015

# Windows
win32 {

}

# Unix
unix:!macx {

    COINCORE_LOCATION = /home/bedeho/JoyStream/Development/libs/mSIGNA/deps/CoinCore

    INCLUDEPATH += $$COINCORE_LOCATION/src

    # Linking
    CONFIG(release, debug|release) {
        LIBS += -L$$COINCORE_LOCATION/lib -llibCoinCore.a
    } else {
        LIBS += -L$$COINCORE_LOCATION/lib -llibCoinCore.a
    }
}

# stdutils
INCLUDEPATH += /home/bedeho/JoyStream/Development/libs/mSIGNA/deps
