/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_CORE_VIEWMODEL_TORRENT_HPP
#define JOYSTREAM_CORE_VIEWMODEL_TORRENT_HPP

#include <core/PluginInstalled.hpp>
#include <core/Stream.hpp>

#include <extension/SellerTorrentPlugin.hpp>
#include <extension/BuyerTorrentPlugin.hpp>

#include <libtorrent/torrent_handle.hpp> // torrent_status
#include <libtorrent/peer_id.hpp> // sha1_hash

namespace joystream {

    namespace extension {

        class SellerTorrentPluginStatus;
        class SellerPeerPluginStatus;
        class BuyerTorrentPluginStatus;
        class BuyerPeerPluginStatus;
    }

    namespace core {
    namespace viewmodel {

        class SellerTorrentPlugin;
        class BuyerTorrentPlugin;

        class Torrent : public QObject {

            Q_OBJECT

        public:

            // Constructor
            // Has no parent since Torrent is not QObject
            Torrent(const libtorrent::sha1_hash & infoHash,
                             const std::string & name,
                             const std::string & savePath,
                             const boost::intrusive_ptr<libtorrent::torrent_info> & torrentFile
                             );
            /**
            Torrent(SellerTorrentPluginViewModel * model);
            Torrent(BuyerTorrentPluginViewModel * model);
            */

            // Destructor
            //~Torrent();

            // Add plugins
            void addPlugin(const joystream::extension::SellerTorrentPluginStatus & status);
            void addPlugin(const joystream::extension::BuyerTorrentPluginStatus & status);

            // Add peers on plugins
            void addPeer(const libtorrent::tcp::endpoint & endPoint, const joystream::extension::SellerPeerPluginStatus & status);
            void addPeer(const libtorrent::tcp::endpoint & endPoint, const joystream::extension::BuyerPeerPluginStatus & status);

            // Remove peer
            void removePeer(const libtorrent::tcp::endpoint & endPoint);

            // Update
            void update(const libtorrent::torrent_status & status);
            void update(const joystream::extension::SellerTorrentPluginStatus & status);
            void update(const joystream::extension::BuyerTorrentPluginStatus & status);

            /**
            // Piece alert
            void pieceRead(const libtorrent::error_code & ec,
                           const boost::shared_array<char> & buffer,
                           int pieceIndex,
                           int size);
            */

            // Getters
            libtorrent::sha1_hash infoHash() const;

            QString name() const;

            QString savePath() const;

            //int totalSize() const;

            //const libtorrent::torrent_info * torrentInfo() const;

            boost::intrusive_ptr<libtorrent::torrent_info> torrentFile() const;

            PluginInstalled pluginInstalled() const;

            libtorrent::torrent_status status() const;

            SellerTorrentPlugin * sellerTorrentPlugin() const;

            BuyerTorrentPlugin * buyerTorrentPlugin() const;

            /**
            // Summary statics from plugin
            quint32 numberOfClassicPeers() const;
            quint32 numberOfObserverPeers() const;
            quint32 numberOfSellerPeers() const;
            quint32 numberOfBuyerPeers() const;
            quint64 balance() const;
            */

            /**
        public slots:

            void getRange(int start, int end);

            void getStart(int start);

            void errorOccured(Stream::Error errorOccured);
            */

        signals:

            // Status changed
            //void pluginInstalledChanged(PluginInstalled pluginInstalled);
            void torrentStatusChanged(const libtorrent::torrent_status & status);

            void startedBuyerTorrentPlugin(const BuyerTorrentPlugin * model);
            void startedSellerTorrentPlugin(const SellerTorrentPlugin * model);

            /**
            // Sends response with data over socket to client
            // * contentType = type of data, e.g. "video/mp4"
            // * start,end = what subrange of total underlying data stream is being transmitted
            // * total = full range of data stream
            // * pieces = actual data
            // * startOffsetInFirstPiece = Offset to start in first piece (inclusive)
            // * stopOffsetInLastPiece = Offset to stop in last piece (inclusive)
            void dataRangeRead(const QString & contentType,
                               int start,
                               int end,
                               int total,
                               const QVector<Piece> & pieces,
                               int startOffsetInFirstPiece,
                               int stopOffsetInLastPiece);

            // A request starting at given position was invalid
            void receivedInvalidRange(int start);

            // Piece with given index is required
            void pieceNeeded(int piece);
            */

            /**
            // Summary statics from plugin
            void numberOfClassicPeersChanged(quint32 num);
            void numberOfObserverPeersChanged(quint32 num);
            void numberOfSellerPeersChanged(quint32 num);
            void numberOfBuyerPeersChanged(quint32 num);
            void balanceChanged(quint64 balance);
            */

            // Action invocation
            void pause(const libtorrent::sha1_hash & infoHash);
            void start(const libtorrent::sha1_hash & infoHash);
            void remove(const libtorrent::sha1_hash & infoHash);

        private:

            // Info hash of torrent
            libtorrent::sha1_hash _infoHash;

            // Name of torrent
            QString _name; // how should this be used?

            // Save path
            QString _savePath;

            // Torrent file
            //libtorrent::torrent_info * _torrentInfo;
            boost::intrusive_ptr<libtorrent::torrent_info> _torrentFile;

            // Type of torrent plugin presently installed on torrent
            PluginInstalled _pluginInstalled;

            // Status of torrent
            libtorrent::torrent_status _status;

            // View model for seller plugin which may be installed
            // Has to be pointer since its QObject.
            // Object is owned by this
            SellerTorrentPlugin * _sellerTorrentPluginViewModel;

            // View model for buyer plugin which may be installed
            // Has to be pointer since its QObject.
            // Object is owned by this
            BuyerTorrentPlugin * _buyerTorrentPluginViewModel;

            void updatePluginInstalled(PluginInstalled mode);
        };

    }
    }
}

#endif // JOYSTREAM_CORE_VIEWMODEL_TORRENT_HPP
