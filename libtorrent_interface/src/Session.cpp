#include "Session.hpp"

torrent_handle Session::add(add_torrent_params const &p) const
{
    _sessionHandle.add_torrent(p);
}

void Session::pause() const
{
    _sessionHandle.pause();
}

void Session::remove() const
{
    _sessionHandle.remove();
}

boost::shared_ptr<aux::session_impl> Session::find(sha1_hash const &info_hash) const
{
    return _sessionHandle.find(info_hash);
}

boost::shared_ptr<aux::session_impl> Session::native_handle() const
{
    return _sessionHandle.native_handle();
}

Session::~Session()
{
    delete _sessionHandle;
}
