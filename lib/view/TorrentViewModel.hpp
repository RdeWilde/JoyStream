#ifndef TORRENT_VIEW_MODEL_HPP
#define TORRENT_VIEW_MODEL_HPP

//#include "extension/PluginMode.hpp"
#include "controller/PluginInstalled.hpp"

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

    // Update view model
    void update(const libtorrent::torrent_status & torrentStatus);
    void updateName(const QString & name);
    void updateSize(int size);
    void updateState(bool paused, libtorrent::torrent_status::state_t state, float progress);
    void updateSpeed(int downloadRate, int uploadRate);
    void updatePeers(int numberOfPeers, int numberOfPeersWithExtension);
    void updatePluginInstalled(PluginInstalled mode);
    void updateBalance(int tokensReceived, int tokensSent);

    /**
    void addPeerPlugin(const libtorrent::tcp::endpoint & endPoint);
    void removePeerPlugin(const libtorrent::tcp::endpoint & endPoint);
    void updatePeerPluginState(PeerPluginStatus status);
    */

    // Pops up context menu at given position
    void showContextMenu(QPoint pos);

    // Getters and setters
    libtorrent::sha1_hash infoHash();

public slots:

    // Action slots for context menu clicks
    void pauseMenuAction();
    void startMenuAction();
    void removeMenuAction();

private:

    // Hash of torrent
    libtorrent::sha1_hash _infoHash;

    // Pointer to main window
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
};

//#include <QMetaType>
//Q_DECLARE_METATYPE(TorrentViewModel *) // QStandardItem::setData(QVariant::fromValue(this))

#endif // TORRENT_VIEW_MODEL_HPP
