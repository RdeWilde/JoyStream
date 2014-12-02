#ifndef TORRENT_VIEW_MODEL_HPP
#define TORRENT_VIEW_MODEL_HPP

#include "extension/PeerPluginId.hpp"

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/torrent_handle.hpp> // libtorrent::torrent_status, torrent_status::state_t

#include <QLoggingCategory>
#include <QMetaType> // Q_DECLARE_METATYPE

#include <map>

#include <boost/asio/ip/tcp.hpp>

class QStandardItemModel;
class QStandardItem;
class PaymentChannelViewModel;
class QString;

class PeerPlugin;
class PeerPluginStatus;

class TorrentViewModel
{

public:

    // Constructor
    TorrentViewModel(const libtorrent::sha1_hash & info_hash, QStandardItemModel * torrentTableViewModel, QLoggingCategory & category);

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

    // Getter
    const libtorrent::sha1_hash & getInfoHash();

private:

    // Hash of torrent
    libtorrent::sha1_hash info_hash_;

    // View model for torrent table
    QStandardItemModel * torrentTableViewModel_;

    // View model for payment channels table
    QStandardItemModel * paymentChannelsTableViewModel_;

    /*
     * ALTER LATER WE SHOULD NOT USE POINTER AS KEY, RATHER IP:PORT OR
     * SOMETHING ELSE STABLE.
     */
    // View models for peers
    std::map<boost::asio::ip::tcp::endpoint ,PaymentChannelViewModel *> paymentChannelViewModels;

    // torrentTableViewModel_ items
    QStandardItem * nameItem,
                  * sizeItem,
                  * stateItem,
                  * speedItem,
                  * peersItem,
                  * modeItem,
                  * paymentChannelsItem,
                  * balanceItem;

    // Logging category
    QLoggingCategory & category_;
};

Q_DECLARE_METATYPE(TorrentViewModel *)

#endif // TORRENT_VIEW_MODEL_HPP
