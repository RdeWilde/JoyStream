#ifndef SESSION_HPP
#define SESSION_HPP

#include <libtorrent/session.hpp>

#include <stdutils/uchar_vector.h>

class Session {
public:
    Session();
    libtorrent::session* s;
};

#endif
