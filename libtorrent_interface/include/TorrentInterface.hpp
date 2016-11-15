#ifndef TORRENT_INTERFACE
#define TORRENT_INTERFACE

#include <boost/shared_ptr.hpp>
#include <libtorrent/torrent.hpp>
#include <libtorrent/torrent_handle.hpp>

class TorrentInterface {
public:
    virtual sha1_hash infoHash() const = 0;
    virtual boost::shared_ptr<torrent> native_handle() const = 0;

    virtual ~TorrentInterface() = 0;
};

#endif //TORRENT_INTERFACE
