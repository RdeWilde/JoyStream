#ifndef PEER_H
#define PEER_H

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <libtorrent/peer_connection.hpp>

#include "PeerInterface.hpp"

typedef boost::system::error_code error_code;

class Peer : public PeerInterface {

public:
    Peer();

    virtual void connect() const;
    virtual bool is_outgoing() const;
    virtual void getPeerInfo(peer_info &p) const;
    virtual boost::shared_ptr<peer_connection> native_handle() const;
    virtual void disconnect(error_code const &ec, operation_t op, int error = 0) const;

    ~Peer();
private:
    peer_connection_handle *m_peer_connection_handle;
};

#endif // PEER_H
