#include "TorrentPluginViewModel.hpp"

TorrentPluginViewModel::TorrentPluginViewModel()
        : _viewExtension("Extension", this) {

    //_torrentTableContextMenu.addAction(&_remove);
    //connect(&_viewExtension, SIGNAL(triggered()), this, SLOT(removeMenuAction()));
    //connect(&_viewExtension, SIGNAL(triggered()), this, SLOT(removeMenuAction()));
}

TorrentPluginViewModel::~TorrentPluginViewModel() {

}

void TorrentPluginViewModel::viewExtensionMenuAction() {
    //qCDebug(_category) << "Not implemented.";
}

libtorrent::sha1_hash TorrentPluginViewModel::infoHash() const {
    return _infoHash;
}

void TorrentPluginViewModel::setInfoHash(const libtorrent::sha1_hash & infoHash){
    _infoHash = infoHash;
}


