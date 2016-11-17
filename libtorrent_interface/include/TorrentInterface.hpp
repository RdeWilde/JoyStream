#ifndef LIBTORRENT_INTERFACE_TORRENT_INTERFACE_HPP
#define LIBTORRENT_INTERFACE_TORRENT_INTERFACE_HPP

#include <boost/shared_ptr.hpp>
#include <libtorrent/torrent.hpp>
#include <libtorrent/torrent_handle.hpp>

namespace joystream {
namespace libtorrent_interface {

class TorrentInterface {
public:
    virtual sha1_hash infoHash() const = 0;
    virtual boost::shared_ptr<torrent> native_handle() const = 0;

    virtual ~TorrentInterface() = 0;
};

}
}

#endif //LIBTORRENT_INTERFACE_TORRENT_INTERFACE_HPP
