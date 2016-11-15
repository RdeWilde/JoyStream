#ifndef PEER_HPP
#define PEER_HPP

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <libtorrent/peer_connection.hpp>

#include "PeerInterface.hpp"

typedef boost::system::error_code error_code;

class Peer : public PeerInterface {

public:
    Peer(const peer_connection_handle* pc) : _peerConnectionHandle(pc) {}

    virtual void connect() const;
    virtual bool is_outgoing() const;
    virtual void getPeerInfo(peer_info &p) const;
    virtual boost::shared_ptr<peer_connection> native_handle() const;
    virtual void disconnect(error_code const &ec, operation_t op, int error = 0) const;

    ~Peer();
private:
    peer_connection_handle *_peerConnectionHandle;
};

#endif // PEER_HPP
