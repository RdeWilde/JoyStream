#include <joystream_libtorrent_session/Session.hpp>
#include <openssl/crypto.h>
#include <common/PrivateKey.hpp>
#include <boost/shared_ptr.hpp>
#include <extension/extension.hpp>

#include <boost/asio/impl/src.hpp>

Session::Session() {
    libtorrent::settings_pack sett;
    sett.set_str(libtorrent::settings_pack::listen_interfaces, "0.0.0.0:6881");
    s = new libtorrent::session(sett);
    libtorrent::error_code ec;
    if (ec) {
        fprintf(stderr, "failed to open listen socket: %s\n", ec.message().c_str());
    } else {
        // Create and install plugin
        boost::shared_ptr<joystream::extension::Plugin> plugin(new joystream::extension::Plugin(60));

        // Add plugin extension
        s->add_extension(boost::static_pointer_cast<libtorrent::plugin>(plugin));

        plugin_ = plugin.get();
    }

    Coin::PrivateKey sk = Coin::PrivateKey::generate();

    printf("OpenSSL version: %s\n", SSLeay_version(SSLEAY_VERSION));
}
