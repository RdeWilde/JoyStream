#ifndef TORRENT_HPP_INCLUDE
#define TORRENT_HPP_INCLUDE

#include "TorrentInterface.hpp"

class Torrent : public TorrentInterface {
public:
    Torrent(const torrent_handle &th) : _torrentHandle(th) {}
    ~Torrent();

    virtual sha1_hash infoHash() const;
    virtual boost::shared_ptr<torrent> native_handle() const;

private:
    torrent_handle _torrentHandle;
}

#endif // TORRENT_HPP_INCLUDE
