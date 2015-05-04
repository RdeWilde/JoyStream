#ifndef SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP
#define SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP

#include "TorrentPluginViewModel.hpp"

#include <libtorrent/socket_io.hpp>

class SellerPeerPluginViewModel;

class SellerTorrentPluginViewModel : public TorrentPluginViewModel
{
public:

    // Default constructor
    SellerTorrentPluginViewModel();

    // Constructor from members
    SellerTorrentPluginViewModel(const libtorrent::sha1_hash & infoHash);

    // Destructor
    ~SellerTorrentPluginViewModel();

private:


    // Maps endpoint to view model for corresponding seller peer plugin
    QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *> _sellerPeerPluginViewModels;




};

#endif // SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP
