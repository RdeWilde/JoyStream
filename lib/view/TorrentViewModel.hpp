#ifndef TORRENT_VIEW_MODEL_HPP
#define TORRENT_VIEW_MODEL_HPP

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/torrent_handle.hpp> // torrent_status::state_t

#include <map>

class QStandardItemModel;
class QStandardItem;
class PaymentChannelViewModel;
class libtorrent::torrent_status;

class PeerPlugin;
class PeerPluginStatus;

class TorrentViewModel
{

public:

    // Constructor
    TorrentViewModel(libtorrent::sha1_hash & info_hash, QStandardItemModel * torrentTableViewModel);

    // Destructor
    ~TorrentViewModel();

    // For altering view model of payment channels table, for example in response to user clicks on torrents
    QStandardItemModel * getPaymentChannelsTableViewModel();

    // Update view-model
    void update(const libtorrent::torrent_status & torrentStatus);
    void updateName(const QString & name);
    void updateSize(int size);
    void updateState(bool paused, libtorrent::torrent_status::state_t state, float progress);
    void updateSpeed(int downloadRate, int uploadRate);
    void updatePeers(int numberOfPeers, int numberOfPeersWithExtension);
    void updateMode(bool pluginOn);
    void updateBalance(int tokensReceived, int tokensSent);

    void addPaymentChannel(PeerPlugin * peerPlugin); // Add view-model for payment channel
    void updatePaymentChannel(PeerPluginStatus status); //

private:

    // Hash of torrent
    libtorrent::sha1_hash info_hash_;

    // View model for torrent table
    QStandardItemModel * torrentTableViewModel_;

    // View model for payment channels table
    QStandardItemModel * paymentChannelsTableViewModel_;

    // View models for peers

    /*
     *
     * ALTER LATER WE SHOULD NOT USE POINTER AS KEY, RATHER IP:PORT OR
     * SOMETHING ELSE STABLE.
     *
     */
    std::map<PeerPlugin *,PaymentChannelViewModel *> paymentChannelViewModels;

    // torrentTableViewModel_ items
    QStandardItem * nameItem,
                  * sizeItem,
                  * stateItem,
                  * speedItem,
                  * peersItem,
                  * modeItem,
                  * paymentChannelsItem,
                  * balanceItem;
};

#endif // TORRENT_VIEW_MODEL_HPP
