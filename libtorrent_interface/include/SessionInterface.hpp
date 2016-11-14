#ifndef TORRENT_SESSION_HANDLE_INTERFACE
#define TORRENT_SESSION_HANDLE_INTERFACE

#include "PeerInterface.hpp"
#include "TorrentInterface.hpp"

#include <libtorrent/session_handle.hpp>

class SessionInterface {
public:
    virtual void add() = 0;
    virtual void find() = 0;
    virtual void pause() = 0;
    virtual void remove() = 0;

    virtual session_handle* native_handle() = 0;
    virtual PeerInterface* getPeerInterface() = 0;
    virtual TorrentInterface* getTorrentInterface() = 0;

    virtual ~SessionInterface() = 0;
private:
    session_handle* m_nativeHandle;

    PeerInterface *m_peerInterface;
    TorrentInterface *m_torrentInterface;
};

#endif //TORRENT_SESSION_HANDLE_INTERFACE
