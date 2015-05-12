#ifndef TORRENT_VIEW_HPP
#define TORRENT_VIEW_HPP

#include <libtorrent/sha1_hash.hpp>

#include <QMenu>
#include <QAction>

class TorrentViewModel;
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
                QStandardItemModel * itemModel);

    // Text conversion routines
    static QString sizeToString(int size) const;
    static QString pluginInstalledToString(PluginInstalled pluginInstalled) const;
    static QString torrentStateToString(bool paused, libtorrent::torrent_status::state_t state, float progress) const;
    static QString speedToString(int downloadRate, int uploadRate) const;
    static QString peersToString(int numberOfPeers, int numberOfPeersWithExtension) const;

    // Getters
    const QMenu * torrentTableContextMenu() const;
    const QAction * pauseAction() const;
    const QAction * startAction() const;
    const QAction * removeAction() const;
    const QAction * viewExtensionAction() const;

public slots:

    // Update
    void updatePluginInstalled(PluginInstalled pluginInstalled);
    void updateStatus(const libtorrent::torrent_status & status);
    void updatePeers(int numberOfPeers, int numberOfPeersWithExtension);

private:

    // Torrent info hash
    // Required as payload for signals
    //libtorrent::sha1_hash _infoHash;

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
