# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, July 29 2015

# Windows
win32 {

}

# Unix
unix:!macx {

    COINQ_LOCATION = /home/bedeho/JoyStream/Development/libs/mSIGNA/deps/CoinQ

    INCLUDEPATH += $$COINQ_LOCATION/src

    # Linking
    CONFIG(release, debug|release) {
        LIBS += -L$$COINQ_LOCATION/lib -llibCoinQ.a
    } else {
        LIBS += -L$$COINQ_LOCATION/lib -llibCoinQ.a
    }
}
