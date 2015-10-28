# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2015

JOYSTREAM_VERSION_MAJOR = 0
JOYSTREAM_VERSION_MINOR = 3
JOYSTREAM_VERSION_PATCH = 2
JOYSTREAM_VERSION_BUILD = 0

win32: VERSION = $${JOYSTREAM_VERSION_MAJOR}.$${JOYSTREAM_VERSION_MINOR}.$${JOYSTREAM_VERSION_PATCH}.$${JOYSTREAM_VERSION_BUILD}
else: VERSION = $${JOYSTREAM_VERSION_MAJOR}.$${JOYSTREAM_VERSION_MINOR}.$${JOYSTREAM_VERSION_PATCH}

DEFINES += APPLICATION_NAME=\'\"JoyStream\"\'
DEFINES += APPLICATION_DESCRIPTION=\'\"A cryptocurrency enabled BitTorrent client\"\'
DEFINES += APPLICATION_VERSION_MAJOR=$${JOYSTREAM_VERSION_MAJOR}
DEFINES += APPLICATION_VERSION_MINOR=$${JOYSTREAM_VERSION_MINOR}
DEFINES += APPLICATION_VERSION_PATCH=$${JOYSTREAM_VERSION_PATCH}
DEFINES += APPLICATION_PARAMETER_FILE_NAME=\'\"parameters.txt\"\'
DEFINES += APPLICATION_BITCOIN_NETWORK=Coin::Network::testnet3
DEFINES += APPLICATION_BLOCKCYPHER_TOKEN=\'\"810b742b6d389ef9e1519d8d844d5747\"\'
