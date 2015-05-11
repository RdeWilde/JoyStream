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
    TorrentView(const TorrentViewModel * viewModel, QStandardItemModel * itemModel);

    // Getters
    QMenu * torrentTableContextMenu();

public slots:

    // Update
    void updatePluginInstalled(PluginInstalled pluginInstalled);
    void updateStatus(const libtorrent::torrent_status & status);
    void updatePeers(int numberOfPeers, int numberOfPeersWithExtension);

private:

    // Torrent info hash
    // Required as payload for signals
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
    QAction _pause,
            _start,
            _remove,
            _viewExtension;

    // Text conversion routines
    QString getSizeText(int size);
    QString getPluginInstalledText(PluginInstalled pluginInstalled);
    QString getTorrentStateText(bool paused, libtorrent::torrent_status::state_t state, float progress);
    QString getSpeedText(int downloadRate, int uploadRate);
    QString getPeersText(int numberOfPeers, int numberOfPeersWithExtension);

};

#endif // TORRENT_VIEW_HPP
