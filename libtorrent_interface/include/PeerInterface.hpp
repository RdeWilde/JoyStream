#ifndef PEER_INTERFACE
#define PEER_INTERFACE

#include "SessionInterface.hpp"
#include "TorrentInterface.hpp"

#include <libtorrent/peer_connection_handle.hpp>

class PeerInterface {
public:
    virtual int pid() = 0;
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isOutgoing() = 0;
    virtual void sendBuffer() = 0;
    virtual char getPeerInfo() = 0;

    virtual peer_connection_handle* native_handle() = 0;
    virtual SessionInterface* getSessionInterface() = 0;
    virtual TorrentInterface* getTorrentInterface() = 0;

    virtual ~PeerInterface() = 0;
private:
    peer_connection_handle *m_nativeHandle;

    SessionInterface *m_sessionInterface;
    TorrentInterface *m_torrentInterface;
}

#endif //PEER_INTERFACE
