#include <joystream_libtorrent_session/Session.hpp>

#include <boost/asio/impl/src.hpp>

Session::Session() {
    libtorrent::settings_pack sett;
    sett.set_str(libtorrent::settings_pack::listen_interfaces, "0.0.0.0:6881");
    s = new libtorrent::session(sett);
    libtorrent::error_code ec;
    if (ec) {
        fprintf(stderr, "failed to open listen socket: %s\n", ec.message().c_str());
    } else {
        std::cout << "Created Session" << std::endl;
    }
}
