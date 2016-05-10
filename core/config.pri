# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2015

DEFINES += CORE_VERSION_MAJOR=0
DEFINES += CORE_VERSION_MINOR=1
DEFINES += CORE_EXTENSION_FINGERPRINT=\\\"BM\\\" # Bedeho Mender, was almost no other ones left (https://wiki.theory.org/BitTorrentSpecification)
DEFINES += CORE_CONTROLLER_POST_TORRENT_UPDATES_DELAY=1000 # Time between each time session.post_torrent_updates() is called, in milliseconds

DEFINES += CORE_CONTROLLER_RECONNECT_DELAY=3000 #Time between reconnection attempts (for spv client or blockcypher websocket client)

DEFINES += CORE_CONTROLLER_SPV_PROTOCOL_ERRORS_BEFORE_RECONNECT=5 # Number of protocol errors allowed before disconnecting from peer
DEFINES += CORE_CONTROLLER_SPV_KEEPALIVE_TIMEOUT=20
