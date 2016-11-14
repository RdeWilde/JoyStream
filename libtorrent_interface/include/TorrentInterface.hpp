#ifndef TORRENT_INTERFACE
#define TORRENT_INTERFACE

#include "PeerInterface.hpp"
#include "SessionInterface.hpp"

#include <libtorrent/torrent_handle.hpp>

class TorrentInterface {
public:
    virtual void infoHash() = 0;

    virtual torrent_handle* native_handle() = 0;
    virtual PeerInterface* getPeerInterface() = 0;
    virtual SessionInterface* getSessionInterface() = 0;

    virtual ~TorrentInterface() = 0;
private:
    torrent_handle *m_nativeHandle;

    PeerInterface *m_peerInterface;
    SessionInterface *m_sessionInterface;
}

#endif //TORRENT_INTERFACE
