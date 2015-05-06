#ifndef TORRENT_VIEW_MODEL_HPP
#define TORRENT_VIEW_MODEL_HPP

//#include "extension/PluginMode.hpp"
#include "controller/PluginInstalled.hpp"
#include "SellerTorrentPluginViewModel.hpp"
#include "BuyerTorrentPluginViewModel.hpp"

#include "extension/SellerTorrentPlugin.hpp"
#include "extension/BuyerTorrentPlugin.hpp"

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/torrent_handle.hpp> // libtorrent::torrent_status, torrent_status::state_t

#include <QLoggingCategory>
#include <QMenu>
#include <QAction>
#include <QStandardItemModel>
#include <QMap>

#include <boost/asio/ip/tcp.hpp>

class QStandardItem;
class QString;
class MainWindow;
class PeerPlugin;
class PeerPluginStatus;
class PeerPluginViewModel;
class Controller;
class SellerTorrentPluginViewModel;
class BuyerTorrentPluginViewModel;

class TorrentViewModel : public QObject
{
    Q_OBJECT

public:

    static const char * columnTitles[];
    static const int numberOfColumns;

    // Constructor
    TorrentViewModel(const libtorrent::sha1_hash & info_hash,
                     Controller * controller,
                     QStandardItemModel * torrentTableViewModel);

    // Destructor
    ~TorrentViewModel();

    // Add plugins
    void addSellerPlugin();
    void addBuyerPlugin();

    // Update view model
    void update(const libtorrent::torrent_status & torrentStatus);
    void updateName(const QString & name);
    void updateSize(int size);
    void updateState(bool paused, libtorrent::torrent_status::state_t state, float progress);
    void updateSpeed(int downloadRate, int uploadRate);
    void updatePeers(int numberOfPeers, int numberOfPeersWithExtension);
    void updateBalance(int tokensReceived, int tokensSent);

    // Update plugin view models
    void update(const BuyerTorrentPlugin::Status & status);
    void update(const SellerTorrentPlugin::Status & status);

    // Pops up context menu at given position
    void showContextMenu(QPoint pos);

    // Getters and setters
    //libtorrent::sha1_hash infoHash();

    PluginInstalled pluginInstalled() const;

public slots:

    // Action slots for context menu clicks
    void pauseMenuAction();
    void startMenuAction();
    void removeMenuAction();
    void viewExtensionMenuAction();

private:

    // Hash of torrent
    libtorrent::sha1_hash _infoHash;

    // Pointer to main window
    // Talk to controller through signals in the future perhaps
    Controller * _controller;

    // Model items, have to be pointers since QStandardItemModel takes ownership of
    // objects and deletes them.
    QStandardItem * _nameItem,
                  * _sizeItem,
                  * _stateItem,
                  * _speedItem,
                  * _peersItem,
                  * _modeItem,
                  * _peerPluginsItem,
                  * _balanceItem;

    // Context menu
    QMenu _torrentTableContextMenu;

    // Context menu actions
    QAction _pause;
    QAction _start;
    QAction _remove;

    /**
     * Try to avoid this sharding in the future, absorb into TorrentviewModel or something..
     * The problem with absorbing into TorrentViewModel is that a torrent does not initially
     * have a plugin, hence the view model for it must support that properly.
     */

    // Type of torrent plugin presently installed on torrent
    PluginInstalled _pluginInstalled;

    // Context menu acton for vieweing extension window
    QAction _viewExtension;

    // View model for seller plugin which may be installed
    // This view model is owned by us
    SellerTorrentPluginViewModel * _sellerTorrentPluginViewModel;

    // View model for buyer plugin which may be installed
    // This view model is owned by us
    BuyerTorrentPluginViewModel * _buyerTorrentPluginViewModel;

    void updatePluginInstalled(PluginInstalled mode);
};

//#include <QMetaType>
//Q_DECLARE_METATYPE(TorrentViewModel *) // QStandardItem::setData(QVariant::fromValue(this))

#endif // TORRENT_VIEW_MODEL_HPP
