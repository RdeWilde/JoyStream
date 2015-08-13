/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/controller/TorrentViewModel.hpp>
#include <core/controller/PeerPluginViewModel.hpp>
#include <core/controller/SellerTorrentPluginViewModel.hpp>
#include <core/controller/BuyerTorrentPluginViewModel.hpp>
#include <core/extension/PeerPlugin.hpp>

#include <QDebug>

TorrentViewModel::TorrentViewModel(const libtorrent::sha1_hash & infoHash,
                                   const std::string & name,
                                   const std::string & savePath,
                                   libtorrent::torrent_info * torrentInfo)
    : _infoHash(infoHash)
    , _name(QString::fromStdString(name))
    , _savePath(QString::fromStdString(savePath))
    , _torrentInfo(torrentInfo)
    , _pluginInstalled(PluginInstalled::None)
    , _sellerTorrentPluginViewModel(NULL)
    , _buyerTorrentPluginViewModel(NULL) {

    /**
    , _fileIndex(0)
    , _total(_torrentInfo->file_at(_fileIndex).size)
    , _contentType("video/mp4")
    , _defaultRangeLength(1024*1024) // 1 MB
    , _currentlyServicingRangeRequest(false) {

    Q_ASSERT(!_torrentInfo->file_at(_fileIndex).pad_file);
    */
}

/**
TorrentViewModel::TorrentViewModel(SellerTorrentPluginViewModel * model)
    : _pluginInstalled(PluginInstalled::Seller)
    , _sellerTorrentPluginViewModel(model)
    , _buyerTorrentPluginViewModel(NULL) {
}

TorrentViewModel::TorrentViewModel(BuyerTorrentPluginViewModel * model)
    : _pluginInstalled(PluginInstalled::Buyer)
    , _sellerTorrentPluginViewModel(NULL)
    , _buyerTorrentPluginViewModel(model) {
}
*/

/**
TorrentViewModel::~TorrentViewModel() {

    switch(_pluginInstalled) {

        case PluginInstalled::Seller:
            delete _sellerTorrentPluginViewModel;
            Q_ASSERT(_buyerTorrentPluginViewModel == NULL);

        case PluginInstalled::Buyer:
            delete _buyerTorrentPluginViewModel;
            Q_ASSERT(_sellerTorrentPluginViewModel == NULL);

        case PluginInstalled::Observer:
            Q_ASSERT(false);

        case PluginInstalled::None:
            Q_ASSERT(_buyerTorrentPluginViewModel == NULL);
            Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
    }
}
*/

void TorrentViewModel::addPlugin(const SellerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Seller;

    _sellerTorrentPluginViewModel = new SellerTorrentPluginViewModel(this, status);

    //emit pluginInstalledChanged(PluginInstalled::Seller);
    emit startedSellerTorrentPlugin(_sellerTorrentPluginViewModel);
}

void TorrentViewModel::addPlugin(const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Buyer;

    // Create view model and save in map
    _buyerTorrentPluginViewModel = new BuyerTorrentPluginViewModel(this, status);

    //emit pluginInstalledChanged(PluginInstalled::Buyer);
    emit startedBuyerTorrentPlugin(_buyerTorrentPluginViewModel);
}

void TorrentViewModel::addPeer(const libtorrent::tcp::endpoint & endPoint, const SellerPeerPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::Seller);
    Q_ASSERT(_sellerTorrentPluginViewModel != NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel == NULL);

    _sellerTorrentPluginViewModel->addPeer(endPoint, status);
}

void TorrentViewModel::addPeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::Buyer);
    Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel != NULL);

    _buyerTorrentPluginViewModel->addPeer(endPoint, status);
}

void TorrentViewModel::removePeer(const libtorrent::tcp::endpoint & endPoint) {

    switch(_pluginInstalled) {

        case PluginInstalled::None:
            Q_ASSERT(false);
            break;

        case PluginInstalled::Buyer:
            Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
            Q_ASSERT(_buyerTorrentPluginViewModel != NULL);
            _buyerTorrentPluginViewModel->removePeer(endPoint);
            break;

        case PluginInstalled::Seller:
            Q_ASSERT(_sellerTorrentPluginViewModel != NULL);
            Q_ASSERT(_buyerTorrentPluginViewModel == NULL);
            _sellerTorrentPluginViewModel->removePeer(endPoint);
            break;

        case PluginInstalled::Observer:
            Q_ASSERT(false);
            break;

        default:
            Q_ASSERT(false);
    }

}

void TorrentViewModel::update(const libtorrent::torrent_status & status) {

    _status = status;
    emit torrentStatusChanged(status);
}

void TorrentViewModel::update(const SellerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::Seller);
    Q_ASSERT(_sellerTorrentPluginViewModel != NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel == NULL);

    _sellerTorrentPluginViewModel->update(status);
}

void TorrentViewModel::update(const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::Buyer);
    Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
    Q_ASSERT(_buyerTorrentPluginViewModel != NULL);

    _buyerTorrentPluginViewModel->update(status);
}

/**
void TorrentViewModel::pieceRead(const libtorrent::error_code & ec,
                                 const boost::shared_array<char> & buffer,
                                 int pieceIndex,
                                 int size) {

    // Do nothing if no range request is being serviced
    if(!_currentlyServicingRangeRequest)
        return;

    // Skip pieces which are not relevant to present request range
    if(pieceIndex < _pieces.front().index() || pieceIndex > _pieces.last().index())
        return;

    // Check that there was no error
    if(ec) {

        qDebug() << ec.message();
        Q_ASSERT(false);
    }

    // Get piece
    Piece & piece = _pieces[pieceIndex - _pieces.front().index()];

    Q_ASSERT(piece.length() == size);

    // Do nothing if we already have read this piece
    if(piece.hasValidData())
        return;
    else {

        // Otherwise

        // Save data
        piece.setData(buffer);

        // Note that we now have valid data
        piece.setHasValidData(true);

        // Count towards number of valid pieces read
        _numberOfOutstandingPieces--;

        Q_ASSERT(_numberOfOutstandingPieces >= 0);

        // Check if we are done, if so send signal and clean up
        if(_numberOfOutstandingPieces == 0) {

            emit dataRangeRead(_contentType,
                               _start,
                               _end,
                               _total,
                               _pieces,
                               _startOffsetInFirstPiece,
                               _stopOffsetInLastPiece);

            // Exit range request servicing mode
            _currentlyServicingRangeRequest = false;
        }
    }
}
*/

libtorrent::sha1_hash TorrentViewModel::infoHash() const {
    return _infoHash;
}

QString TorrentViewModel::name() const {
    return _name;
}

QString TorrentViewModel::savePath() const {
    return _savePath;
}

const libtorrent::torrent_info * TorrentViewModel::torrentInfo() const {
    return _torrentInfo;
}

PluginInstalled TorrentViewModel::pluginInstalled() const {
    return _pluginInstalled;
}

libtorrent::torrent_status TorrentViewModel::status() const {
    return _status;
}

SellerTorrentPluginViewModel * TorrentViewModel::sellerTorrentPluginViewModel() const {
    return _sellerTorrentPluginViewModel;
}

BuyerTorrentPluginViewModel * TorrentViewModel::buyerTorrentPluginViewModel() const {
    return _buyerTorrentPluginViewModel;
}

/**
void TorrentViewModel::getRange(int start, int end) {

    // Check that the requested range is valid
    if(start < 0 || end >= _total || end <= start) {

        // Send notification
        emit receivedInvalidRange(_start);

        return;
    }

    //
    // Note that any previously active request will simply
    // be overwritten below
    //

    if(_currentlyServicingRangeRequest)
        qDebug() << "Dropping currently active request for [" << _start << "," << _end << "]";

    // Note that request is being serviced
    _currentlyServicingRangeRequest = true;

    // Save range
    _start = start;
    _end = end;

    // Figure out how many full pieces must be read
    int rangeSize = _end - _start + 1;

    // Figure out what piece and offset this the request range in this file starts at
    libtorrent::peer_request r = _torrentInfo->map_file(_fileIndex, _start, rangeSize);

    Q_ASSERT(r.length == rangeSize);

    // Iterate through all pieces corresponding to given range [_start, _end] of file
    // and setup the required state to process the piece reads,
    // as well as requesting piece reads
    _numberOfOutstandingPieces = 0;
    _startOffsetInFirstPiece = r.start;
    _pieces.clear();

    int firstPieceSize = _torrentInfo->piece_size(r.piece);

    // Treat a range which is within a single piece as an exception
    if(r.start + r.length <= firstPieceSize) { // NB: rangeSize > firstPieceSize does not test this

        _numberOfOutstandingPieces = 1;
        _stopOffsetInLastPiece = r.start + r.length - 1;
        _pieces.push_back(Piece(r.piece, firstPieceSize, false, boost::shared_array<char>(NULL)));

        // Request that piece is read
        emit pieceNeeded(r.piece);

    } else {

        int requiredDataInFirstPiece = firstPieceSize - r.start;

        // Figure out how many pieces in total are touched by range
        int numberOfPiecesAfterFirst = (r.length - requiredDataInFirstPiece) / _torrentInfo->piece_length();
        _numberOfOutstandingPieces = 1 + numberOfPiecesAfterFirst;

        for(int i = r.piece;i < _numberOfOutstandingPieces;i++) {

            // Get size of this piece, where
            // last piece may be shorter than rest, which are uniform
            int pieceSize = _torrentInfo->piece_size(i);

            // Crate piece object
            _pieces.push_back(Piece(i, pieceSize, false, boost::shared_array<char>(NULL)));

            // Request that piece is read
            emit pieceNeeded(i);
        }

        Q_ASSERT(_pieces.size() == _numberOfOutstandingPieces);

        // How much of range oveflows into last piece
        _stopOffsetInLastPiece = (r.length - requiredDataInFirstPiece) % _torrentInfo->piece_length();
    }
}

void TorrentViewModel::getStart(int start) {
    getRange(start, start + _defaultRangeLength);
}

void TorrentViewModel::errorOccured(Stream::Error errorOccured) {
    qDebug() << "TorrentViewModel::errorOccured";
}

quint32 TorrentViewModel::numberOfClassicPeers() {

}

quint32 TorrentViewModel::numberOfObserverPeers() {

}

quint32 TorrentViewModel::numberOfSellerPeers() {

}

quint32 TorrentViewModel::numberOfBuyerPeers() {

}

quint64 TorrentViewModel::balance() {

}
*/

/**
void TorrentViewModel::showExtensionDialog() {

    Q_ASSERT(_pluginInstalled != PluginInstalled::None);

    switch(_pluginInstalled) {

        case PluginInstalled::Seller:
            Q_ASSERT(_sellerTorrentPluginViewModel != NULL);
            Q_ASSERT(_buyerTorrentPluginViewModel == NULL);

            emit sellerTorrentPluginDialogRequested(_sellerTorrentPluginViewModel);

        case PluginInstalled::Buyer:
            Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
            Q_ASSERT(_buyerTorrentPluginViewModel != NULL);

            emit buyerTorrentPluginDialogRequested(_buyerTorrentPluginViewModel);

        case PluginInstalled::Observer:
            Q_ASSERT(false);

        case PluginInstalled::None:
            Q_ASSERT(_buyerTorrentPluginViewModel == NULL);
            Q_ASSERT(_sellerTorrentPluginViewModel == NULL);
    }
}
*/
