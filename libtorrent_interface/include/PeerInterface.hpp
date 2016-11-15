#ifndef PEER_INTERFACE
#define PEER_INTERFACE

#include <boost/shared_ptr.hpp>
#include <libtorrent/peer_connection_handle.hpp>

class PeerInterface {
public:
    virtual ~PeerInterface() = 0;

    virtual int pid() const = 0;
    virtual void connect() const = 0;
    virtual void disconnect() const = 0;
    virtual bool isOutgoing() const = 0;
    virtual void sendBuffer() const = 0;
    virtual char getPeerInfo() const = 0;

    virtual boost::shared_ptr<peer_connection> native_handle() const = 0;
};

#endif //PEER_INTERFACE
