/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef EXTENSION_SELLER_PEER_PLUGIN_HPP
#define EXTENSION_SELLER_PEER_PLUGIN_HPP

#include <extension/PeerPlugin.hpp>
#include <extension/SellerPeerPluginPeerState.hpp>
#include <extension/SellerPeerPluginClientState.hpp>
#include <paymentchannel/PayeeConfiguration.hpp>
#include <paymentchannel/Payee.hpp>

#include <QSet>
#include <QVector>

#include <boost/shared_array.hpp> // can't forward declare

namespace libtorrent {
    struct disk_io_job;
}

namespace joystream {

namespace paymentchannel {
    class PayeeConfiguration;
}

namespace extension {

    class SellerTorrentPlugin;
    enum class PluginMode;

    class SellerPeerPlugin : public PeerPlugin {

    public:

        // Constructor
        SellerPeerPlugin(SellerTorrentPlugin * torrentPlugin,
                         libtorrent::bt_peer_connection * connection,
                         bool scheduledForDeletingInNextTorrentPluginTick,
                         const joystream::paymentchannel::PayeeConfiguration & payeeConfiguration,
                         int numberOfPieces,
                         QLoggingCategory & category);

        // Destructor
        virtual ~SellerPeerPlugin();

        // Libtorrent callbacks
        char const * type() const;
        virtual void on_disconnect(libtorrent::error_code const & ec);
        virtual void on_connected();
        virtual bool on_extension_handshake(libtorrent::lazy_entry const & handshake);
        //virtual bool on_extension_handshake(const libtorrent::bdecode_node & handshake);
        virtual bool on_have(int index);
        virtual bool on_bitfield(libtorrent::bitfield const & bitfield);
        virtual bool on_have_all();
        virtual bool on_reject(libtorrent::peer_request const & peerRequest);
        virtual bool on_request(libtorrent::peer_request const & peerRequest);
        virtual bool on_unchoke();
        virtual bool on_interested();
        virtual bool on_allowed_fast(int index);
        virtual bool on_have_none();
        virtual bool on_choke();
        virtual bool on_not_interested();
        virtual bool on_piece(libtorrent::peer_request const & piece, libtorrent::disk_buffer_holder & data);
        virtual bool on_suggest(int index);
        virtual bool on_cancel(libtorrent::peer_request const & peerRequest);
        virtual bool on_dont_have(int index);
        virtual void sent_unchoke();
        virtual bool can_disconnect(libtorrent::error_code const & ec);
        virtual bool on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body);
        virtual void on_piece_pass(int index);
        virtual void on_piece_failed(int index);
        virtual void tick();
        virtual bool write_request(libtorrent::peer_request const & peerRequest);

        // Handler used by block read call
        //void disk_async_read_handler(int block, int, const libtorrent::disk_io_job & job);

        // Handler for piece call back, piece data is not owned by us,
        // and is therefore not freed
        void pieceRead(int piece, const boost::shared_array<char> & pieceData, int size);

        // Called when piece reading failed
        void pieceReadFailed(int piece);

        // Closes the peer connection
        void close_connection();

        // Returns transaction for last received valid payment
        Coin::Transaction lastPaymentTransaction() const;

        // Getters and setters
        virtual PluginMode mode() const;
        virtual quint64 balance() const;

        SellerPeerPluginPeerState peerState() const;
        void setPeerState(const SellerPeerPluginPeerState &peerState);

        SellerPeerPluginClientState clientState() const;
        void setClientState(const SellerPeerPluginClientState &clientState);

        Payee payee() const;

        int lastRequestedFullPiece() const;
        void setLastRequestedFullPiece(int lastRequestedFullPiece);

        int lastRequestedFullPieceSize() const;
        void setLastRequestedFullPieceSize(int lastRequestedFullPieceSize);

        quint32 numberOfAsyncReadsCompleted() const;
        void setNumberOfAsyncReadsCompleted(quint32 numberOfAsyncReadsCompleted);

        QVector<libtorrent::disk_io_job> completedAsyncReads() const;
        void setCompletedAsyncReads(const QVector<libtorrent::disk_io_job> &completedAsyncReads);

        quint32 totalNumberOfPieces() const;
        void setTotalNumberOfPieces(quint32 totalNumberOfPieces);

    private:

        // Torrent level plugin
        // Should we use a boost::shared_ptr instead since object lifetime is managed by it?
        SellerTorrentPlugin * _plugin;

        // State of peer
        SellerPeerPluginPeerState _peerState;

        // State of client
        SellerPeerPluginClientState _clientState;

        // Payee portion of payment channel
        joystream::paymentchannel::Payee _payee;

        /**
         * Seller terms


        // Piece price (in satoshi units)
        quint32 _minPrice;

        // When refund is spendable at the earliest
        quint32 _minLock;

        // Maximum number of sellers accepted in contract
        quint32 _maxSellers;

         */

        /**
         * Torrent information and status
         */

        // Meta data in torrent file
        //libtorrent::torrent_info _torrentFile;

        // Block size used
        //int _blockSize;

        // Number of pieces in torrent file
        // used to check validity of full piece requests
        int _numberOfPieces;

        /**
         * Request/Piece management
         */

        // Last full piece request: The piece currently being read from disk/transmitted
        //int _lastRequestedFullPiece;

        // Full pieces sent
        QList<int> _fullPiecesSent;

        // Requests received but not serviced, in order
        //QList<libtorrent::peer_request> _sPendingRequests;

        // Requests serviced, in order
        //QList<libtorrent::peer_request> _serviced;

        /**
         * Piece reading management:
         * The values in this section are only relevant when
         * _clientState == ClientState::reading_piece_from_disk


        // The number of reads completed so far
        quint32 _numberOfAsyncReadsCompleted;

        // Vector of block read operations, where read operation is located
        // in position of corresponding block number.
        QVector<libtorrent::disk_io_job> _completedAsyncReads;
        */

        /**
         *  Processess message
         */
        virtual void processObserve(const Observe * m);
        virtual void processBuy(const Buy * m);
        virtual void processSell(const Sell * m);
        virtual void processJoinContract(const JoinContract * m);
        virtual void processJoiningContract(const JoiningContract * m);
        virtual void processSignRefund(const SignRefund * m);
        virtual void processRefundSigned(const RefundSigned * m);
        virtual void processReady(const Ready * m);
        virtual void processRequestFullPiece(const RequestFullPiece * m);
        virtual void processFullPiece(const FullPiece * m);
        virtual void processPayment(const Payment * m);

        // Resets plugin in response to peer sending a mode message
        void peerModeReset();
    };

}
}

#endif // EXTENSION_SELLER_PEER_PLUGIN_HPP
