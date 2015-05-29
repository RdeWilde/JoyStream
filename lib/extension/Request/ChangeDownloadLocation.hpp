#ifndef CHANGE_DOWNLOAD_LOCATION_HPP
#define CHANGE_DOWNLOAD_LOCATION_HPP

#include "TorrentPluginRequest.hpp"

class ChangeDownloadLocation : public TorrentPluginRequest
{
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

#endif // CHANGE_DOWNLOAD_LOCATION_HPP
