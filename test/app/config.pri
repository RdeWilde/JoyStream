# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2015

# Windows
win32 {
    ROOT_DIRECTORY = "C:/JoyStream/"
}

# Mac
macx {
    ROOT_DIRECTORY = "/Users/mokhtar/JoyStream/"
}

# Unix
unix:!macx {
    ROOT_DIRECTORY = "/home/bedeho/JoyStream/"
}

# Network to use for testing
DEFINES += BITCOIN_NETWORK=Coin::Network::testnet3

# Location where test clients create their home folders,
# i.e. where they load/save actual torrent data
HOME_LOCATION = $${ROOT_DIRECTORY}"HOME/"

# Location where wallets are saved/loaded
WALLET_LOCATION = $${ROOT_DIRECTORY}"WALLETS/"

# Location torrent files are loaded from
TORRENT_LOCATION = $${ROOT_DIRECTORY}"TORRENTS/"

# Location block header store is saved from in SPV mode
BLOCKSTORE_LOCATION = $${ROOT_DIRECTORY}"BLOCKSTORE/"

# Location
DEFINES += HOME_LOCATION=\'\"$${HOME_LOCATION}\"\'
DEFINES += WALLET_LOCATION=\'\"$${WALLET_LOCATION}\"\'
DEFINES += TORRENT_LOCATION=\'\"$${TORRENT_LOCATION}\"\'
DEFINES += BLOCKSTORE_LOCATION=\'\"$${BLOCKSTORE_LOCATION}\"\'


# Torrent files
RISE_AND_RISE_OF_BITCOIN_FILE = "The.Rise.and.Rise.of.Bitcoin.2014.720p.HDrip.x264.AAC.MVGroup.org.mp4.torrent"
LITTLE_SIMZ_FILE = "Little Simz - AGE 101- DROP 1 - 01 Homosapiens VS Aliens.mp3.torrent"

DEFINES += RISE_AND_RISE_OF_BITCOIN_FILE=\'\"$${TORRENT_LOCATION}$${RISE_AND_RISE_OF_BITCOIN_FILE}\"\'
DEFINES += LITTLE_SIMZ_FILE=\'\"$${TORRENT_LOCATION}$${LITTLE_SIMZ_FILE}\"\'
