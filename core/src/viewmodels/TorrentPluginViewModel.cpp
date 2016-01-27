/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/controller/TorrentPluginViewModel.hpp>

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


