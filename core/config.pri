# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2015

DEFINES += CORE_VERSION_MAJOR=0
DEFINES += CORE_VERSION_MINOR=1
DEFINES += CORE_EXTENSION_FINGERPRINT=\\\"BM\\\" # Bedeho Mender, was almost no other ones left (https://wiki.theory.org/BitTorrentSpecification)
DEFINES += CORE_EXTENSION_NAME=\\\"btc_payment\\\" # extension name
DEFINES += CORE_EXTENSION_VERSION=1
DEFINES += CORE_CONTROLLER_POP_ALERTS_DELAY=200 # Time between each time session.pop_alerts() is called, in milliseconds
DEFINES += CORE_CONTROLLER_POST_TORRENT_UPDATES_DELAY=1000 # Time between each time session.post_torrent_updates() is called, in milliseconds
DEFINES += CORE_CONTROLLER_PROTOCOL_ERRORS_BEFORE_RECONNECT=5 # Number of protocol errors allowed before disconnecting from peer
DEFINES += CORE_CONTROLLER_RECONNECT_DELAY=10000 #Time between reconnection attempts
