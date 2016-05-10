/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef TORRENT_PLUGIN_VIEW_MODEL_HPP
#define TORRENT_PLUGIN_VIEW_MODEL_HPP

#include <libtorrent/sha1_hash.hpp>

#include <QMap>
#include <QStandardItemModel>

/**
 * Need for this class seems to be questionable, limited se of polymorphism
 *
 * I DONT THINK THIS CLASS IS EVEN USED ANYMORE !!!
 */
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
