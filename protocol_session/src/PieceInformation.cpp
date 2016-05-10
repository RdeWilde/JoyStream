/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 24 2016
 */

#include <protocol_session/PieceInformation.hpp>

namespace joystream {
namespace protocol_session {

PieceInformation::PieceInformation(int index, unsigned int size, bool downloaded)
    : _index(index)
    , _size(size)
    , _downloaded(downloaded) {
}

int PieceInformation::index() const {
    return _index;
}

void PieceInformation::setIndex(int index) {
    _index = index;
}

unsigned int PieceInformation::size() const {
    return _size;
}

void PieceInformation::setSize(unsigned int size) {
    _size = size;
}

bool PieceInformation::downloaded() const {
    return _downloaded;
}

void PieceInformation::setDownloaded(bool downloaded) {
    _downloaded = downloaded;
}

}
}
