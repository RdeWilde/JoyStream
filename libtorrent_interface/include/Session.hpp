#ifndef SESSION_HPP
#define SESSION_HPP

#include "SessionInterface.hpp"

class Session : public SessionInterface {
public:
    Session(const session_handle &sh) : _sessionHandle(sh) {}
    ~Session();

    virtual void pause() const;
    virtual void remove() const;
    virtual torrent_handle add(add_torrent_params const &params) const;
    virtual boost::shared_ptr<torrent> find(sha1_hash const &info_hash) const;

    virtual boost::shared_ptr<aux::session_impl*> native_handle() const;

private:
    session_handle _sessionHandle;
};

#endif //SESSION_HPP
