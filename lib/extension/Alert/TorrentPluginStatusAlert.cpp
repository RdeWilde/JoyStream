#include "TorrentPluginStatusAlert.hpp"


TorrentPluginStatusAlert::TorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash
                                                   ,int numberOfPeers
                                                   ,int numberOfPeersWithExtension
                                                   ,bool pluginOn
                                                   ,int tokensReceived
                                                   ,int tokensSent)
    : TorrentPluginAlert(infoHash)
    ,_numberOfPeers(numberOfPeers)
    ,_numberOfPeersWithExtension(numberOfPeersWithExtension)
    ,_pluginOn(pluginOn)
    ,_tokensReceived(tokensReceived)
    ,_tokensSent(tokensSent){

}

int TorrentPluginStatusAlert::type() const {
    return alert_type;
}

char const * TorrentPluginStatusAlert::what() const {
    return "TorrentPluginStatusAlert";
}

std::string TorrentPluginStatusAlert::message() const {
    return std::string("TorrentPluginStatusAlert::message: IMPLEMENT LATER");
}

int TorrentPluginStatusAlert::category() const {
    return libtorrent::alert::stats_notification;
}

std::auto_ptr<libtorrent::alert> TorrentPluginStatusAlert::clone() const {
    return std::auto_ptr<alert>(new TorrentPluginStatusAlert(*this));
}

int TorrentPluginStatusAlert::numberOfPeers() const {
    return _numberOfPeers;
}

int TorrentPluginStatusAlert::numberOfPeersWithExtension() const {
    return _numberOfPeersWithExtension;
}

bool TorrentPluginStatusAlert::pluginOn() const {
    return _pluginOn;
}

int TorrentPluginStatusAlert::tokensReceived() const {
    return _tokensReceived;
}

int TorrentPluginStatusAlert::tokensSent() const {
    return _tokensSent;
}
