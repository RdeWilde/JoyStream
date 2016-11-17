#ifndef LIBTORRENT_INTERFACE_SESSION_INTERFACE_HPP
#define LIBTORRENT_INTERFACE_SESSION_INTERFACE_HPP

#include <boost/shared_ptr.hpp>
#include <libtorrent/torrent.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/session_handle.hpp>
#include <libtorrent/aux_/session_impl.hpp>

class SessionInterface {
public:
    virtual void pause() = 0;
    virtual void remove() = 0;
    virtual torrent_handle add(add_torrent_params const &params) = 0;
    virtual boost::shared_ptr<torrent> find(sha1_hash const &info_hash) = 0;

    virtual boost::shared_ptr<aux::session_impl> native_handle() = 0;
    virtual ~SessionInterface() = 0;
};

#endif // LIBTORRENT_INTERFACE_SESSION_INTERFACE_HPP
