#ifndef TORRENT_VIEW_MODEL_HPP
#define TORRENT_VIEW_MODEL_HPP

#include "extension/PeerPluginId.hpp"

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/torrent_handle.hpp> // libtorrent::torrent_status, torrent_status::state_t

#include <QLoggingCategory>
#include <QMetaType> // Q_DECLARE_METATYPE
#include <QStandardItemModel>

#include <map>

#include <boost/asio/ip/tcp.hpp>

class QStandardItem;
class QString;

class PeerPlugin;
class PeerPluginStatus;
class PeerPluginViewModel;

class TorrentViewModel
{

public:

    static const char * columnTitles[];
    static const int numberOfColumns;

    // Constructor
    TorrentViewModel(const libtorrent::sha1_hash & info_hash, QStandardItemModel & torrentTableViewModel, QLoggingCategory & category);

    // Destructor
    ~TorrentViewModel();

    // For altering view model of peer plugins table, for example in response to user clicks on torrents
    QStandardItemModel * getPeerPluginsTableViewModel();

    // Update
    void update(const libtorrent::torrent_status & torrentStatus);
    void updateName(const QString & name);
    void updateSize(int size);
    void updateState(bool paused, libtorrent::torrent_status::state_t state, float progress);
    void updateSpeed(int downloadRate, int uploadRate);
    void updatePeers(int numberOfPeers, int numberOfPeersWithExtension);
    void updateMode(bool pluginOn);
    void updateBalance(int tokensReceived, int tokensSent);

    void addPeerPlugin(PeerPlugin * peerPlugin);
    void removePeerPlugin(const boost::asio::ip::tcp::endpoint & endPoint);
    void updatePeerPluginState(PeerPluginStatus status); //

    // Getter
    const libtorrent::sha1_hash & getInfoHash() const;

private:

    // Hash of torrent
    libtorrent::sha1_hash info_hash_;

    // View model for torrent table. Is reference since it is shared
    // across objects of this type.
    QStandardItemModel & torrentTableViewModel_;

    // View model for peer plugins table
    QStandardItemModel peerPluginsTableViewModel_;

    // View models for peers
    std::map<boost::asio::ip::tcp::endpoint, PeerPluginViewModel *> peerPluginViewModels;

    // Model items, have to be pointers since QStandardItemModel takes ownership of
    // objects and deletes them.
    QStandardItem * nameItem,
                  * sizeItem,
                  * stateItem,
                  * speedItem,
                  * peersItem,
                  * modeItem,
                  * peerPluginsItem,
                  * balanceItem;

    // Logging category
    QLoggingCategory & category_;
};

Q_DECLARE_METATYPE(TorrentViewModel *)

#endif // TORRENT_VIEW_MODEL_HPP
