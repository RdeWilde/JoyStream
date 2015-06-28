/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef REQUEST_FULL_PIECE_HPP
#define REQUEST_FULL_PIECE_HPP

#include "ExtendedMessagePayload.hpp"

class RequestFullPiece : public ExtendedMessagePayload
{
public:

    // Default constructor
    RequestFullPiece();

    // Constructor based on members
    RequestFullPiece(int pieceIndex);

    // Constructor based on raw payload
    RequestFullPiece(QDataStream & stream);

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

    // Getters and setters
    int pieceIndex() const;
    void setPieceIndex(int pieceIndex);

private:

    // Index of piece being requested
    int _pieceIndex;
};

#endif // REQUEST_FULL_PIECE_HPP
