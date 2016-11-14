#include "Peer.h"

Peer::Peer()
{

}

bool Peer::is_outgoing() const
{
    return m_peer_connection_handle->is_outgoing();
}

void Peer::getPeerInfo(peer_info &p) const
{
    m_peer_connection_handle->get_peer_info(p);
}

void Peer::connect() const
{
    m_peer_connection_handle->connect();
}

void Peer::disconnect(const error_code &ec, operation_t op, int error) const
{
    m_peer_connection_handle->disconnect(ec, op, error);
}

boost::shared_ptr<peer_connection> Peer::native_handle() const
{
    return m_peer_connection_handle->native_handle();
}
