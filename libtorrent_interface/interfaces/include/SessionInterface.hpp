#ifndef LIBTORRENT_INTERFACE_SESSION_INTERFACE_HPP
#define LIBTORRENT_INTERFACE_SESSION_INTERFACE_HPP

#include "TorrentInterface.hpp"

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/session_handle.hpp>
#include <libtorrent/add_torrent_params.hpp>

namespace libtorrent {
    class sha1_hash;

    struct torrent_handle;
    struct session_handle;
    struct add_torrent_params;
}

namespace joystream {
namespace libtorrent_interface {

class SessionInterface {
public:
    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual libtorrent::session_handle native_handle() = 0;
    virtual TorrentInterface* find(libtorrent::sha1_hash const &info_hash) = 0;
    virtual TorrentInterface* add(libtorrent::add_torrent_params const &params) = 0;

    virtual ~SessionInterface() {}
};

}
}

#endif // LIBTORRENT_INTERFACE_SESSION_INTERFACE_HPP
