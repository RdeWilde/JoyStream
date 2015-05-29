#include "ChangeDownloadLocation.hpp"

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


