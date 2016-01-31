/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef FULLPIECE_HPP
#define FULLPIECE_HPP

#include <protocol/ExtendedMessagePayload.hpp>

#include <boost/shared_array.hpp>

namespace joystream {
namespace protocol {

    class FullPiece : public ExtendedMessagePayload {

    public:

        // Default constructor
        FullPiece();

        // Constructor from members
        FullPiece(const boost::shared_array<char> & piece, int length);

        // Constructor based on raw payload
        // NB: Substitute with static factory in future, so that you cannot create stale
        // payload objects if there is an error in the reading from stream
        FullPiece(QDataStream & stream, int length);

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(QDataStream & stream) const;

        // Getters and setters
        boost::shared_array<char> piece() const;
        void setPiece(const boost::shared_array<char> & piece);

        void setLength(int length);

    private:

        // Piece data;
        boost::shared_array<char> _piece;

        // Byte length of piece
        int _length;

    };

}
}

#endif // FULLPIECE_HPP
