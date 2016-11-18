#ifndef LIBTORRENT_INTERFACE_PEER_INTERFACE_HPP
#define LIBTORRENT_INTERFACE_PEER_INTERFACE_HPP

#include <boost/shared_ptr.hpp>

#include <libtorrent/peer_id.hpp>
#include <libtorrent/peer_info.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/operations.hpp>
#include <libtorrent/peer_connection_handle.hpp>

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

class PeerInterface {
public:
    virtual ~PeerInterface() = 0;

    virtual bool isOutgoing() const = 0;

    virtual void connect() const = 0;
    virtual void getPeerInfo(libtorrent::peer_info &p) const = 0;
    virtual void sendBuffer(char const *begin, int size, int flags = 0) const = 0;
    virtual void disconnect(libtorrent::error_code const &ec, libtorrent::operation_t op, int error = 0) const = 0;

    virtual libtorrent::peer_id const& pid() const = 0;
    virtual libtorrent::peer_connection_handle native_handle() const = 0;
};

}
}

#endif //LIBTORRENT_INTERFACE_PEER_INTERFACE_HPP
