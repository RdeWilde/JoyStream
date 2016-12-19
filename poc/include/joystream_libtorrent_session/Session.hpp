#ifndef SESSION_HPP
#define SESSION_HPP

#include <libtorrent/session.hpp>
#include <extension/extension.hpp>
#include <stdutils/uchar_vector.h>

class Session {
public:
    Session();
    libtorrent::session* s;
    joystream::extension::Plugin* plugin_;
};

#endif
