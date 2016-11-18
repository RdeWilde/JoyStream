#include "Peer.hpp"

namespace joystream {
namespace libtorrent_interface {


bool Peer::is_outgoing() const
{
    return _peerConnectionHandle.is_outgoing();
}

void Peer::connect() const
{
    _peerConnectionHandle.connect();
}

void Peer::getPeerInfo(libtorrent::peer_info &p) const
{
    _peerConnectionHandle.get_peer_info(p);
}

void Peer::sendBuffer(const char *begin, int size, int flags=0) const
{
    _peerConnectionHandle.send_buffer(begin, size, flags);
}

void Peer::disconnect(const libtorrent::error_code &ec, libtorrent::operation_t op, int error) const
{
    _peerConnectionHandle.disconnect(ec, op, error);
}

libtorrent::peer_id const& Peer::pid() const
{
    _peerConnectionHandle.pid();
}

libtorrent::peer_connection_handle Peer::native_handle() const
{
    return _peerConnectionHandle;
}

Peer::~Peer()
{
}

}
}
