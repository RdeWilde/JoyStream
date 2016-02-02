/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOSTREAM_EXTENSION_BUYER_PEER_PLUGIN_HPP
#define JOSTREAM_EXTENSION_BUYER_PEER_PLUGIN_HPP

#include <extension/PeerPlugin.hpp>
#include <extension/BuyerPeerPluginPeerState.hpp>
#include <extension/BuyerPeerPluginClientState.hpp>

#include <QSet>

namespace joystream {
namespace extension {

    class BuyerTorrentPlugin;
    enum class PluginMode;

    class BuyerPeerPlugin : public PeerPlugin {

    public:

        // Constructor
        BuyerPeerPlugin(BuyerTorrentPlugin * plugin,
                        libtorrent::bt_peer_connection * connection,
                        const std::string & bep10ClientIdentifier,
                        bool scheduledForDeletingInNextTorrentPluginTick,
                        QLoggingCategory & category);

        // Destructor
        virtual ~BuyerPeerPlugin();

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

        //quint32 refillPipeline();

        void close_connection();

        // Getters and setters
        BuyerPeerPluginPeerState peerState() const;
        void setPeerState(const BuyerPeerPluginPeerState & peerState);

        BuyerPeerPluginClientState clientState() const;
        void setClientState(BuyerPeerPluginClientState clientState);

        quint32 payorSlot() const;
        void setPayorSlot(quint32 payorSlot);

        //bool assignedPiece() const;
        //void setAssignedPiece(bool assignedPiece);

        int indexOfAssignedPiece() const;
        void setIndexOfAssignedPiece(int indexOfAssignedPiece);

        /**
        int pieceSize() const;
        void setPieceSize(int pieceSize);

        int blockSize() const;
        void setBlockSize(int blockSize);

        int numberOfBlocksInPiece() const;
        void setNumberOfBlocksInPiece(int numberOfBlocksInPiece);

        int numberOfBlocksRequested() const;
        void setNumberOfBlocksRequested(int numberOfBlocksRequested);

        int numberOfBlocksReceived() const;
        void setNumberOfBlocksReceived(int numberOfBlocksReceived);
        */

        QSet<libtorrent::peer_request> unservicedRequests() const;
        void setUnservicedRequests(const QSet<libtorrent::peer_request> &unservicedRequests);

        QDateTime whenLastRequestServiced() const;
        void setWhenLastRequestServiced(const QDateTime &whenLastRequestServiced);

        QList<int> downloadedPieces() const;
        void setDownloadedPieces(const QList<int> &downloadedPieces);

        void addDownloadedPiece(int index);

        quint64 totalAmountSent() const;

        virtual PluginMode mode() const;
        virtual quint64 balance() const;

    private:

        // Torrent level plugin
        // Should we use a boost::shared_ptr instead since object lifetime is managed by it?
        BuyerTorrentPlugin * _plugin;

        // State of peer
        BuyerPeerPluginPeerState _peerState;

        // State of client
        BuyerPeerPluginClientState _clientState;

        // Peer plugin position in Payor, only valid if
        // _clientState ">" ignored_join_contract_from_peer

        /**
         * WHY IS THIS HERE??
         * I cant see a clear benefit at this writing moment, much
         * better then peer plugin asks torrent plugin to act w.r.t
         * payment channel on its behalf by just using this, using
         * an integer seems unsafe, as one can easily tamper with
         * payment slots of other peers.
         *
         */
        quint32 _payorSlot;

        /**
         * Request/Piece management
         */

        // Whether this peer plugin has been assigned to try to buy a piece from peer seller
        // NOT NEEDED: Should be inferred from ClientState
        //bool _assignedPiece;

        // Index of a piece assigned to this peer, only valid if
        // _clientState == ClientState::waiting_for_full_piece or ClientState::waiting_for_libtorrent_to_validate_piece
        int _indexOfAssignedPiece;

        // Byte length of presently assigned piece
        //int _pieceSize;

        // Byte length of blocks
        //int _blockSize;

        // The number of blocks in the presently assigned piece
        //int _numberOfBlocksInPiece;

        // The number of blocks requested
        //int _numberOfBlocksRequested;

        // The number of blocks which were requested and were subsequently received in a piece message
        //int _numberOfBlocksReceived;

        // Block indexes for block requests which have been issued, but not responded to
        //QSet<libtorrent::peer_request> _unservicedRequests;

        // Time when last FullPiece was serviced
        //QDateTime _whenLastSentFullPiece;

        // Piece indexes, in download order, of
        // all valid pieces downloaded from seller peer during this session
        QList<int> _downloadedValidPieces;

        /**
         * In the future the next two pipeline variables have to be
         * dynamically adjusted based on connection latency and
         * downstream bandwidth.


        // The maximum number of requests which can be pipelined
        const static quint32 _requestPipelineLength = 10;

        // Refill pipline when it falls below this bound
        const static quint32 _requestPipelineRefillBound = 5;
        */

        // Processess message
        virtual void processObserve(const joystream::protocol::Observe * m);
        virtual void processBuy(const joystream::protocol::Buy * m);
        virtual void processSell(const joystream::protocol::Sell * m);
        virtual void processJoinContract(const joystream::protocol::JoinContract * m);
        virtual void processJoiningContract(const joystream::protocol::JoiningContract * m);
        virtual void processSignRefund(const joystream::protocol::SignRefund * m);
        virtual void processRefundSigned(const joystream::protocol::RefundSigned * m);
        virtual void processReady(const joystream::protocol::Ready * m);
        virtual void processRequestFullPiece(const joystream::protocol::RequestFullPiece * m);
        virtual void processFullPiece(const joystream::protocol::FullPiece * m);
        virtual void processPayment(const joystream::protocol::Payment * m);

        // Resets plugin in response to peer sending a mode message
        void peerModeReset();
    };

}
}

#endif // JOSTREAM_EXTENSION_BUYER_PEER_PLUGIN_HPP
