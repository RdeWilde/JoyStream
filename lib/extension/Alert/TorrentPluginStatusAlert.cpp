#include "TorrentPluginStatusAlert.hpp"


TorrentPluginStatusAlert::TorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash
                                                   ,int numberOfPeers
                                                   ,int numberOfPeersWithExtension
                                                   ,bool pluginStarted
                                                   ,int tokensReceived
                                                   ,int tokensSent
                                                   ,PluginMode mode)
    : TorrentPluginAlert(infoHash)
    ,_numberOfPeers(numberOfPeers)
    ,_numberOfPeersWithExtension(numberOfPeersWithExtension)
    ,_pluginStarted(pluginStarted)
    ,_tokensReceived(tokensReceived)
    ,_tokensSent(tokensSent)
    ,_pluginMode(mode) {

}

TorrentPluginStatusAlert::TorrentPluginStatusAlert(const TorrentPluginStatusAlert & torrentPluginStatusAlert)
    :TorrentPluginAlert(torrentPluginStatusAlert.getInfoHash())
    ,_numberOfPeers(torrentPluginStatusAlert.numberOfPeers())
    ,_numberOfPeersWithExtension(torrentPluginStatusAlert.numberOfPeersWithExtension())
    ,_pluginStarted(torrentPluginStatusAlert.pluginStarted())
    ,_tokensReceived(torrentPluginStatusAlert.tokensReceived())
    ,_tokensSent(torrentPluginStatusAlert.tokensSent())
    ,_pluginMode(torrentPluginStatusAlert.mode()) {

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

bool TorrentPluginStatusAlert::pluginStarted() const {
    return _pluginStarted;
}

int TorrentPluginStatusAlert::tokensReceived() const {
    return _tokensReceived;
}

int TorrentPluginStatusAlert::tokensSent() const {
    return _tokensSent;
}

PluginMode TorrentPluginStatusAlert::mode() const {
    return _pluginMode;
}
