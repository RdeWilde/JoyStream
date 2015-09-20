# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, August 12 2015

# Windows
win32 {
    INCLUDEPATH += /usr/i686-w64-mingw32/

    LIBS += \
            -L/usr/i686-w64-mingw32/lib \
            -lcrypto \
            -lssl
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

# Mac
macx {
        #use openssl which comes with OSX
        OPENSSL_LOCATION = /usr/local/opt/openssl

	INCLUDEPATH += $$OPENSSL_LOCATION/include

	LIBS += -L$$OPENSSL_LOCATION/lib \
		-lcrypto \
		-lssl \
		-ldl
}
