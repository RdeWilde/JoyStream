#include "Peer.h"

bool Peer::is_outgoing() const
{
    return _peerConnectionHandle->is_outgoing();
}

void Peer::getPeerInfo(peer_info &p) const
{
    _peerConnectionHandle->get_peer_info(p);
}

void Peer::connect() const
{
    _peerConnectionHandle->connect();
}

void Peer::disconnect(const error_code &ec, operation_t op, int error) const
{
    _peerConnectionHandle->disconnect(ec, op, error);
}

boost::shared_ptr<peer_connection> Peer::native_handle() const
{
    return _peerConnectionHandle->native_handle();
}
