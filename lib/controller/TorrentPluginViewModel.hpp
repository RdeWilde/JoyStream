#ifndef TORRENT_PLUGIN_VIEW_MODEL_HPP
#define TORRENT_PLUGIN_VIEW_MODEL_HPP

#include <libtorrent/sha1_hash.hpp>

#include <QAction>
#include <QMap>
#include <QStandardItemModel>

// Need for this class seems to be questionable, limited se of polymorphism
class TorrentPluginViewModel : public QObject
{
    Q_OBJECT

public:

    static const char * columnTitles[];
    static const int numberOfColumns;

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



private:

    // Torrent info hash (perhaps remove later)
    libtorrent::sha1_hash _infoHash;

    // View model for peer plugin table
    //QStandardItemModel _peerPluginTableViewModel;


};

#endif // TORRENT_PLUGIN_VIEW_MODEL_HPP
