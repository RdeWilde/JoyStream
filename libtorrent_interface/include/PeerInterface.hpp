#ifndef LIBTORRENT_INTERFACE_PEER_INTERFACE_HPP
#define LIBTORRENT_INTERFACE_PEER_INTERFACE_HPP

#include <boost/shared_ptr.hpp>
#include <libtorrent/peer_connection_handle.hpp>

namespace joystream {
namespace libtorrent_interface {

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

}
}

#endif //LIBTORRENT_INTERFACE_PEER_INTERFACE_HPP
