#include "SellerPeerPluginRemovedAlert.hpp"

SellerPeerPluginRemovedAlert::SellerPeerPluginRemovedAlert(const libtorrent::sha1_hash & infoHash,
                                                           const libtorrent::tcp::endpoint & endPoint)
    :  TorrentPluginAlert(infoHash)
    , _endPoint(endPoint) {
}

SellerPeerPluginRemovedAlert::SellerPeerPluginRemovedAlert(const SellerPeerPluginRemovedAlert & alert)
    : SellerPeerPluginRemovedAlert(alert.infoHash(), alert.endPoint()) {
}

int SellerPeerPluginRemovedAlert::type() const {
    return alert_type;
}

char const* SellerPeerPluginRemovedAlert::what() const {
    return "SellerPeerPluginRemovedAlert";
}

std::string SellerPeerPluginRemovedAlert::message() const {
    return std::string("SellerPeerPluginRemovedAlert::message: IMPLEMENT LATER");
}

int SellerPeerPluginRemovedAlert::category() const {
    return libtorrent::alert::status_notification;
}

std::auto_ptr<libtorrent::alert> SellerPeerPluginRemovedAlert::clone() const {
    return std::auto_ptr<libtorrent::alert>(new SellerPeerPluginRemovedAlert(*this));
}

libtorrent::tcp::endpoint SellerPeerPluginRemovedAlert::endPoint() const {
    return _endPoint;
}
