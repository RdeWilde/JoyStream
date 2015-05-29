#ifndef TORRENT_VIEW_HPP
#define TORRENT_VIEW_HPP

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/torrent_handle.hpp> // libtorrent::torrent_status::state_t

#include <QMenu>
#include <QAction>

class QStandardItem;
enum class PluginInstalled;
class TorrentViewModel;
class BuyerTorrentPluginViewModel;
class SellerTorrentPluginViewModel;

namespace libtorrent {
    struct torrent_status;
}

class TorrentView : public QObject
{
    Q_OBJECT

public:

    // Constructor
    TorrentView(QObject * parent,
                const TorrentViewModel * torrentViewModel,
                QStandardItem * nameItem,
                QStandardItem * sizeItem,
                QStandardItem * stateItem,
                QStandardItem * speedItem,
                QStandardItem * buyersItem,
                QStandardItem * sellersItem,
                QStandardItem * pluginInstalledItem,
                QStandardItem * balanceItem);

    // Text conversion routines
    static QString sizeToString(qint64 size);
    static QString pluginInstalledToString(PluginInstalled pluginInstalled);
    static QString torrentStateToString(bool paused, libtorrent::torrent_status::state_t state, float progress);
    static QString speedToString(int downloadRate, int uploadRate);
    static QString peersToString(int numberOfPeers, int numberOfPeersWithExtension);
    static QString balanceToString(quint64 balance);

public slots:

    // Update
    void updatePluginInstalled(PluginInstalled pluginInstalled);

    void updateStartedBuyerTorrentPlugin(const BuyerTorrentPluginViewModel * model);
    void updateStartedSellerTorrentPlugin(const SellerTorrentPluginViewModel * model);

    void updateStatus(const libtorrent::torrent_status & status);
    //void updatePeers(int numberOfPeers, int numberOfPeersWithExtension);
    void updateSize(qint64 totalSize);
    void updateNumberOfBuyers(quint32 num);
    void updateNumberOfSellers(quint32 num);
    void updateBalance(quint64 balance);

    // Popup context menu in given point
    void showContextMenu(const QPoint & point);

    /**
     * Ui event related slots, e.g. from menus and buttons
     */

    // _torrentTableContextMenu context QMenu
    void pause();
    void start();
    void remove();
    void viewExtension();
    void startStreamPlayback();

signals:

    //void pluginInstalled(const libtorrent::sha1_hash & infoHash);

    /**
     * Ui event related signals corresponding with slots above,
     * where torrent identitiy is added.
     */

    // _torrentTableContextMenu context QMenu
    void pauseTorrentRequested(const libtorrent::sha1_hash & infoHash);
    void startTorrentRequested(const libtorrent::sha1_hash & infoHash);
    void removeTorrentRequested(const libtorrent::sha1_hash & infoHash);
    void requestedViewingExtension(const libtorrent::sha1_hash & infoHash);
    void requestedStreamingPlayback(const libtorrent::sha1_hash & infoHash);

private:

    // Info hash of torrent
    // Is required to send signals based on user interaction
    libtorrent::sha1_hash _infoHash;

    // View model pointers
    // Objects are owned by QStandardItemModel, not us
    QStandardItem * _nameItem,
                  * _sizeItem,
                  * _stateItem,
                  * _speedItem,
                  * _buyersItem,
                  * _sellersItem,
                  * _pluginInstalledItem,
                  * _balanceItem;

    // Context menu
    QMenu _torrentTableContextMenu;

    // Context menu actions
    // Seems to have to be pointers, since they need parent
    QAction _pauseAction,
            _startAction,
            _removeAction,
            _viewExtensionAction,
            _streamAction;
};

#endif // TORRENT_VIEW_HPP
