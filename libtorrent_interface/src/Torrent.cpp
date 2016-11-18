#include "Torrent.hpp"

namespace joystream {
namespace libtorrent_interface {

sha1_hash Torrent::infoHash() const
{
    return _torrentHandle.infoHash();
}

boost::shared_ptr<torrent> Torrent::native_handle() const
{
    return _torrentHandle.native_handle();
}

Torrent::~Torrent()
{
}

}
}
