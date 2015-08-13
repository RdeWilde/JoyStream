/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Request/ChangeDownloadLocation.hpp>

ChangeDownloadLocation::ChangeDownloadLocation(const libtorrent::sha1_hash & infoHash, int pieceIndex)
    : TorrentPluginRequest(infoHash)
    , _pieceIndex(pieceIndex) {
}

TorrentPluginRequestType ChangeDownloadLocation::getTorrentPluginRequestType() const {
    return TorrentPluginRequestType::ChangeDownloadLocation;
}

int ChangeDownloadLocation::pieceIndex() const {
    return _pieceIndex;
}

void ChangeDownloadLocation::setPieceIndex(int pieceIndex) {
    _pieceIndex = pieceIndex;
}


