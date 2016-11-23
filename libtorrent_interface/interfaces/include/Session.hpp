#ifndef LIBTORRENT_INTERFACE_SESSION_HPP
#define LIBTORRENT_INTERFACE_SESSION_HPP

#include "SessionInterface.hpp"
#include "Torrent.hpp"

namespace libtorrent {
    class sha1_hash;

    struct torrent_handle;
    struct session_handle;
    struct add_torrent_params;
}

namespace joystream {
namespace libtorrent_interface {

class Session : public SessionInterface {
public:
    Session(const libtorrent::session_handle& sh);
    ~Session();

    virtual void pause();
    virtual void resume();
    virtual Torrent* add(libtorrent::add_torrent_params const &params);
    virtual Torrent* find(libtorrent::sha1_hash const &info_hash) const;

    virtual libtorrent::session_handle native_handle() const;

private:
    Torrent _torrent;
    libtorrent::session_handle _sessionHandle;
};

}
}

#endif // LIBTORRENT_INTERFACE_SESSION_HPP
