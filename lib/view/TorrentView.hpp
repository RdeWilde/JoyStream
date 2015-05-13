#ifndef TORRENT_VIEW_HPP
#define TORRENT_VIEW_HPP

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/torrent_handle.hpp> // libtorrent::torrent_status::state_t

#include <QMenu>
#include <QAction>

class QStandardItemModel;
class QStandardItem;
enum class PluginInstalled;

namespace libtorrent {
    struct torrent_status;
}

class TorrentView : public QObject
{
    Q_OBJECT

public:

    // Constructor
    TorrentView(const libtorrent::torrent_status & status,
                int size,
                PluginInstalled pluginInstalled,
                QStandardItemModel * model);

    // Text conversion routines
    static QString sizeToString(int size);
    static QString pluginInstalledToString(PluginInstalled pluginInstalled);
    static QString torrentStateToString(bool paused, libtorrent::torrent_status::state_t state, float progress);
    static QString speedToString(int downloadRate, int uploadRate);
    static QString peersToString(int numberOfPeers, int numberOfPeersWithExtension);

    // Getters
    const QAction * pauseAction() const;
    const QAction * startAction() const;
    const QAction * removeAction() const;

public slots:

    // Update
    void updatePluginInstalled(PluginInstalled pluginInstalled);
    void updateStatus(const libtorrent::torrent_status & status);
    void updatePeers(int numberOfPeers, int numberOfPeersWithExtension);

    // Popup context menu in given point
    void showContextMenu(const QPoint & point);

    // Shows extension dialog
    // Slot is primarily meant for _viewExtensionAction::triggered()
    void viewExtension();

signals:

    // Issued when viewExtension() slot is called
    void requestedViewingExtension(const libtorrent::sha1_hash & infoHash);

private:

    // Info hash of torrent
    // Is required to send signals based on user interaction
    libtorrent::sha1_hash _infoHash;

    // View model pointers
    // Objects are owned by QStandardItemModel passed to ctr
    QStandardItem * _nameItem,
                  * _sizeItem,
                  * _stateItem,
                  * _speedItem,
                  * _peersItem,
                  * _modeItem,
                  * _balanceItem;

    // Context menu
    QMenu _torrentTableContextMenu;

    // Context menu actions
    // Seems to have to be pointers, since they need parent
    QAction _pauseAction,
            _startAction,
            _removeAction,
            _viewExtensionAction;
};

#endif // TORRENT_VIEW_HPP
