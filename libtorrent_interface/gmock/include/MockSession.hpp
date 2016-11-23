#ifndef GMOCK_MOCK_SESSION_HPP
#define GMOCK_MOCK_SESSION_HPP

#include "gmock/gmock.h"
#include "SessionInterface.hpp"

namespace joystream {
namespace gmock {

class MockSession : public libtorrent_interface::SessionInterface {
public:
    MOCK_METHOD0(pause, void());
    MOCK_METHOD0(resume, void());
    MOCK_METHOD0(native_handle, libtorrent::session_handle());
    MOCK_METHOD1(find, libtorrent_interface::TorrentInterface*(libtorrent::sha1_hash const &));
    MOCK_METHOD1(add, libtorrent_interface::TorrentInterface*(libtorrent::add_torrent_params const &));
};

}
}

#endif // GMOCK_MOCK_SESSION_HPP
