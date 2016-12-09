#ifndef JOYSTREAM_EXTENSION_SESSION_HPP
#define JOYSTREAM_EXTENSION_SESSION_HPP

#include <extension/Torrent.hpp>
#include <extension/interface/SessionInterface.hpp>

namespace libtorrent {
    class sha1_hash;

    struct torrent_handle;
    struct session_handle;
    struct add_torrent_params;
}

namespace joystream {
namespace extension {

class Session : public interface::SessionInterface {
public:
    Session(const libtorrent::session_handle& sh);
    ~Session();

    virtual void pause();
    virtual void resume();
    virtual Torrent* add(libtorrent::add_torrent_params const &params);
    virtual Torrent* find(libtorrent::sha1_hash const &info_hash) const;

    virtual libtorrent::session_handle native_handle() const;

private:
    libtorrent::session_handle _sessionHandle;
};

}
}

#endif // JOYSTREAM_EXTENSION_SESSION_HPP
