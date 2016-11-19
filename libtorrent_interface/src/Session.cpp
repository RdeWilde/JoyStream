#include "Session.hpp"

namespace joystream {
namespace libtorrent_interface {

void Session::pause() const
{
    _sessionHandle.pause();
}

void Session::remove() const
{
    _sessionHandle.remove();
}

torrent_handle Session::add(libtorrent::add_torrent_params const &p) const
{
    _sessionHandle.add_torrent(p);
}

libtorrent::torrent_handle Session::find(libtorrent::sha1_hash const &info_hash) const
{
    return _sessionHandle.find_torrent(info_hash);
}

libtorrent::session_handle Session::native_handle() const
{
    return _sessionHandle.native_handle();
}

Session::~Session()
{
}

}
}
