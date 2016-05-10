/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 24 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_PIECEINFORMATION_HPP
#define JOYSTREAM_PROTOCOLSESSION_PIECEINFORMATION_HPP

namespace joystream {
namespace protocol_session {

class PieceInformation {

public:

    PieceInformation(int index, unsigned int size, bool downloaded);

    // Getter & setters
    int index() const;
    void setIndex(int index);

    unsigned int size() const;
    void setSize(unsigned int size);

    bool downloaded() const;
    void setDownloaded(bool downloaded);

private:

    // Index of piece
    unsigned int _index;

    // Byte length of given piece
    unsigned int _size;

    // Download status
    bool _downloaded;
};

}
}

#endif // JOYSTREAM_PROTOCOLSESSION_PIECEINFORMATION_HPP
