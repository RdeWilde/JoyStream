/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_REQUEST_CHANGE_DOWNLOAD_LOCATION_HPP
#define JOYSTREAM_EXTENSION_REQUEST_CHANGE_DOWNLOAD_LOCATION_HPP

#include <extension/request/TorrentPluginRequest.hpp>

namespace joystream {
namespace extension {
namespace request {

    class ChangeDownloadLocation : public TorrentPluginRequest {

    public:

        // Constructor
        ChangeDownloadLocation(const libtorrent::sha1_hash & infoHash, int pieceIndex);

        // Returns request type
        virtual TorrentPluginRequestType getTorrentPluginRequestType() const;

        // Getters and setters
        int pieceIndex() const;
        void setPieceIndex(int pieceIndex);

    private:

        // Piece to start downloading from
        int _pieceIndex;

    };

}
}
}

#endif // JOYSTREAM_EXTENSION_REQUEST_CHANGE_DOWNLOAD_LOCATION_HPP
