/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 24 2016
 */

#include <protocol_session/TorrentPieceInformation.hpp>
#include <protocol_session/Exceptions.hpp>

namespace joystream {
namespace protocol_session {

TorrentPieceInformation::TorrentPieceInformation() {
}

void TorrentPieceInformation::addPiece(const PieceInformation & p) {

    // Make sure piece index is valid
    if(p.index() > 0 && (unsigned)p.index() == _pieces.size())
        _pieces.push_back(p);
    else
        throw exception::InvalidPieceIndexException(_pieces.size(), p.index());
}

std::vector<PieceInformation> TorrentPieceInformation::pieces() const {
    return _pieces;
}

}
}
