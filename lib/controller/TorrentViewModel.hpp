#ifndef TORRENT_VIEW_MODEL_HPP
#define TORRENT_VIEW_MODEL_HPP

//#include "extension/PluginMode.hpp"
#include "controller/PluginInstalled.hpp"
#include "extension/SellerTorrentPlugin.hpp"
#include "extension/BuyerTorrentPlugin.hpp"
#include "Stream.hpp"

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/torrent_handle.hpp> // libtorrent::torrent_status, torrent_status::state_t

class SellerTorrentPluginViewModel;
class BuyerTorrentPluginViewModel;

class TorrentViewModel : public QObject
{
    Q_OBJECT

public:

    // Constructor
    // Has no parent since Torrent is not QObject
    TorrentViewModel(const libtorrent::sha1_hash & infoHash,
                     const std::string & name,
                     const std::string & savePath,
                     libtorrent::torrent_info * torrentInfo);
    /**
    TorrentViewModel(SellerTorrentPluginViewModel * model);
    TorrentViewModel(BuyerTorrentPluginViewModel * model);
    */

    // Destructor
    //~TorrentViewModel();

    // Add plugins
    void addPlugin(const SellerTorrentPlugin::Status & status);
    void addPlugin(const BuyerTorrentPlugin::Status & status);

    // Add peers on plugins
    void addPeer(const libtorrent::tcp::endpoint & endPoint, const SellerPeerPlugin::Status & status);
    void addPeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status);

    // Remove peer
    void removePeer(const libtorrent::tcp::endpoint & endPoint);

    // Update
    void update(const libtorrent::torrent_status & status);
    void update(const SellerTorrentPlugin::Status & status);
    void update(const BuyerTorrentPlugin::Status & status);

    // Piece alert
    void pieceRead(const libtorrent::error_code & ec,
                   const boost::shared_array<char> & buffer,
                   int piece,
                   int size);

    // Getters
    libtorrent::sha1_hash infoHash() const;
    QString name() const;
    QString savePath() const;
    const libtorrent::torrent_info * torrentInfo() const;
    PluginInstalled pluginInstalled() const;
    libtorrent::torrent_status status() const;
    SellerTorrentPluginViewModel * sellerTorrentPluginViewModel() const;
    BuyerTorrentPluginViewModel * buyerTorrentPluginViewModel() const;

    /**
    // Summary statics from plugin
    quint32 numberOfClassicPeers() const;
    quint32 numberOfObserverPeers() const;
    quint32 numberOfSellerPeers() const;
    quint32 numberOfBuyerPeers() const;
    quint64 balance() const;
    */

public slots:

    void getRange(int start, int end);

    void getStart(int start);

    void errorOccured(Stream::Error errorOccured);

signals:

    // Status changed
    //void pluginInstalledChanged(PluginInstalled pluginInstalled);
    void torrentStatusChanged(const libtorrent::torrent_status & status);

    void startedBuyerTorrentPlugin(const BuyerTorrentPluginViewModel * model);
    void startedSellerTorrentPlugin(const SellerTorrentPluginViewModel * model);

    // Sends response with data over socket to client
    // * contentType = type of data, e.g. "video/mp4"
    // * start,end = what subrange of total underlying data stream is being transmitted
    // * total = full range of data stream
    // * pieces = actual data
    // * offsetInFirstPiece = offset to start writing from in first piece
    // * offsetInLastPiece
    void dataRangeRead(const QString & contentType,
                       int start,
                       int end,
                       int total,
                       const QVector<Piece> & pieces,
                       int offsetInFirstPiece,
                       int offsetInLastPiece);

    // A request starting at given position was invalid
    void receivedInvalidRange(int start);

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
    libtorrent::torrent_info * _torrentInfo;

    // Type of torrent plugin presently installed on torrent
    PluginInstalled _pluginInstalled;

    // Status of torrent
    libtorrent::torrent_status _status;

    // View model for seller plugin which may be installed
    // Has to be pointer since its QObject.
    // Object is owned by this
    SellerTorrentPluginViewModel * _sellerTorrentPluginViewModel;

    // View model for buyer plugin which may be installed
    // Has to be pointer since its QObject.
    // Object is owned by this
    BuyerTorrentPluginViewModel * _buyerTorrentPluginViewModel;

    void updatePluginInstalled(PluginInstalled mode);

    /**
     *
     * Piece reading related state
     *
     */

    int _firstPieceWhereStreamBegins;
    int _lastPieceWhereStreamEnds;

    // The number of bytes
    int _defaultRangeLength;

    // Content type of content
    QString _contentType;

    // Whether range request is being serviced
    bool _currentlyServicingRangeRequest;

    ////
    //// Everything below here is computed from the last request
    //// and is kept around.
    ////

    // Most recent request
    int _start, _end;

    // Start of range begins in this piece
    int _firstPieceIndex;

    // The total number of pieces required
    int _numberOfPiecesNeeded;

    // Where to start in first piece
    int _offsetInFirstPiece;

    // What index to stop in
    int _offsetInLastPiece;

    QVector<Piece> _pieces;

};

#endif // TORRENT_VIEW_MODEL_HPP
