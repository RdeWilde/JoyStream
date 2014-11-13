
#include "BrPaymentPlugin.hpp"

#include <iostream>


boost::shared_ptr<libtorrent::torrent_plugin> BrPaymentPlugin::new_torrent(libtorrent::torrent * newTorrent, void * userData) {

    std::cout << "Torrent added!" << std::endl;

    return boost::shared_ptr<libtorrent::torrent_plugin>(NULL); //new BrPaymentPeerPlugin(*newTorrent)
}

void BrPaymentPlugin::added(libtorrent::aux::session_impl * session) {

}

void BrPaymentPlugin::on_alert(libtorrent::alert const * a) {

}

void BrPaymentPlugin::on_tick() {

}

bool BrPaymentPlugin::on_optimistic_unchoke(std::vector<libtorrent::policy::peer*> & peers) {
    return false;
}

void BrPaymentPlugin::save_state(libtorrent::entry & stateEntry) const {

}

void BrPaymentPlugin::load_state(libtorrent::lazy_entry const & stateEntry) {

}
