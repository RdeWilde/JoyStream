#include "TorrentPluginViewModel.hpp"

const char * TorrentPluginViewModel::columnTitles[] = {"Peer", "State", "Balance"};
const int TorrentPluginViewModel::numberOfColumns = sizeof(TorrentPluginViewModel::columnTitles)/sizeof(char *);

TorrentPluginViewModel::TorrentPluginViewModel() {
}

TorrentPluginViewModel::~TorrentPluginViewModel() {
}

/**
void TorrentPluginViewModel::viewExtensionMenuAction() {
    //qCDebug(_category) << "Not implemented.";
}
*/

libtorrent::sha1_hash TorrentPluginViewModel::infoHash() const {
    return _infoHash;
}

void TorrentPluginViewModel::setInfoHash(const libtorrent::sha1_hash & infoHash){
    _infoHash = infoHash;
}


