#ifndef LIBTORRENT_INTERFACE_PEER_HPP
#define LIBTORRENT_INTERFACE_PEER_HPP

#include "PeerInterface.hpp"

namespace joystream {
namespace libtorrent_interface {

typedef boost::system::error_code error_code;

class Peer : public PeerInterface {

public:
    Peer(const peer_connection_handle &pc) : _peerConnectionHandle(pc) {}
    ~Peer();

    virtual void connect() const;
    virtual bool is_outgoing() const;
    virtual void getPeerInfo(peer_info &p) const;
    virtual boost::shared_ptr<peer_connection> native_handle() const;
    virtual void disconnect(error_code const &ec, operation_t op, int error = 0) const;

private:
    peer_connection_handle _peerConnectionHandle;
};

}
}

#endif // LIBTORRENT_INTERFACE_PEER_HPP
