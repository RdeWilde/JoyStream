# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 12 2015

# Windows
win32 {

}

# Unix
unix:!macx {

    INCLUDEPATH += /usr/include/openssl

    LIBS += \
            -L/usr/lib/x86_64-linux-gnu \
            -lcrypto \
            -lssl \
            -ldl
}
