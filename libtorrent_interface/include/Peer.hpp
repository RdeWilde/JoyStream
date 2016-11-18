#ifndef LIBTORRENT_INTERFACE_PEER_HPP
#define LIBTORRENT_INTERFACE_PEER_HPP

#include "PeerInterface.hpp"

namespace libtorrent {
    enum operation_t;

    struct peer_info;
    struct peer_connection_handle;

    class sha1_hash;

    typedef sha1_hash peer_id;
    typedef boost::system::error_code error_code;
}

namespace joystream {
namespace libtorrent_interface {

class Peer : public PeerInterface {

public:
    Peer(const libtorrent::peer_connection_handle &pc)
        : _peerConnectionHandle(pc)
    {}
    ~Peer();

    virtual bool is_outgoing() const;

    virtual void connect() const;
    virtual void getPeerInfo(libtorrent::peer_info &p) const;
    virtual void sendBuffer(char const *begin, int size, int flags = 0) const;
    virtual void disconnect(libtorrent::error_code const &ec, libtorrent::operation_t op, int error = 0) const;

    virtual libtorrent::peer_id const& pid() const;
    virtual libtorrent::peer_connection_handle native_handle() const;

private:
    libtorrent::peer_connection_handle _peerConnectionHandle;
};

}
}

#endif // LIBTORRENT_INTERFACE_PEER_HPP
