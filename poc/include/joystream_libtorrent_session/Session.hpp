#ifndef SESSION_HPP
#define SESSION_HPP

#include <libtorrent/session.hpp>

class Session {
public:
    Session();
    libtorrent::session s;
};

#endif
