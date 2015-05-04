#ifndef TORRENT_PLUGIN_VIEW_MODEL_HPP
#define TORRENT_PLUGIN_VIEW_MODEL_HPP

#include <libtorrent/sha1_hash.hpp>

#include <QAction>
#include <QMap>

class TorrentPluginViewModel : public QObject
{
    Q_OBJECT

public:

    // Default constructor
    TorrentPluginViewModel();

    // Construct from members
    TorrentPluginViewModel(const libtorrent::sha1_hash & infoHash);

    // Destructor
    ~TorrentPluginViewModel();

    // Getters and setters
    libtorrent::sha1_hash infoHash() const;
    void setInfoHash(const libtorrent::sha1_hash & infoHash);

public slots:

    void viewExtensionMenuAction();

private:

    // Torrent info hash (perhaps remove later)
    libtorrent::sha1_hash _infoHash;

    // Context menu acton for vieweing extension window
    QAction _viewExtension;
};

#endif // TORRENT_PLUGIN_VIEW_MODEL_HPP
