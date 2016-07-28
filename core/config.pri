# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2015

# Two byte peer-id use in initial BitTorrent handshake
#JS = JoyStream, although previosuly used by now defunct 'JS' - Justseed.it client (https://justseed.it/)
DEFINES += CORE_PEER_ID=\\\"JS\\\"
DEFINES += CORE_VERSION_MAJOR=0
DEFINES += CORE_VERSION_MINOR=1

# Used for tracker communication and BEP10 extended handshake client identification
DEFINES += CORE_USER_AGENT_NAME=\\\"JoyStream\\\"

# Lowest extended message id which the extension should use not to conflict with
# other plugins
DEFINES += CORE_MINIMUM_EXTENDED_MESSAGE_ID=60

# ** To be moved out of core
DEFINES += CORE_CONTROLLER_RECONNECT_DELAY=3000 #Time between reconnection attempts (for spv client or blockcypher websocket client)
DEFINES += CORE_CONTROLLER_SPV_PROTOCOL_ERRORS_BEFORE_RECONNECT=5 # Number of protocol errors allowed before disconnecting from peer
DEFINES += CORE_CONTROLLER_SPV_KEEPALIVE_TIMEOUT=20
