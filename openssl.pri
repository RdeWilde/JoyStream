# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 12 2015

##########################
# openssl : <THIS *HAS* TO BE INCLUDED AFTER mSIGNA, OTHERWISE YOU ARE F***D
##########################

# Windows
win32 {

}

# Unix
unix:!macx {

    OPENSSL_LOCATION = /usr/lib/x86_64-linux-gnu

    INCLUDEPATH += $$OPENSSL_LOCATION/include

    LIBS += -L$$OPENSSL_LOCATION/lib \
            -lcrypto \
            -lssl \
            -ldl
}
