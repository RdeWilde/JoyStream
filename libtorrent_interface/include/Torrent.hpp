#ifndef LIBTORRENT_INTERFACE_TORRENT_HPP
#define LIBTORRENT_INTERFACE_TORRENT_HPP

#include "TorrentInterface.hpp"

namespace libtorrent {
    class sha1_hash;
    class torrent_handle;
}

namespace joystream {
namespace libtorrent_interface {

class Torrent : public TorrentInterface {
public:
    Torrent(const libtorrent::torrent_handle &th) : _torrentHandle(th) {}
    ~Torrent();

    virtual libtorrent::sha1_hash infoHash() const;
    virtual libtorrent::torrent_handle native_handle() const;

private:
    torrent_handle _torrentHandle;
};

}
}

#endif // LIBTORRENT_INTERFACE_TORRENT_HPP
