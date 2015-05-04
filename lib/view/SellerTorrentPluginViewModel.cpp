#include "SellerTorrentPluginViewModel.hpp"

SellerTorrentPluginViewModel::SellerTorrentPluginViewModel(){

}

SellerTorrentPluginViewModel::SellerTorrentPluginViewModel(const libtorrent::sha1_hash & infoHash)
    : TorrentPluginViewModel(infoHash) {
}

SellerTorrentPluginViewModel::~SellerTorrentPluginViewModel() {

}

