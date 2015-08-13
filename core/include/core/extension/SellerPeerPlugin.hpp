/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SELLER_PEER_PLUGIN_HPP
#define SELLER_PEER_PLUGIN_HPP

#include <core/extension/PeerPlugin.hpp>
#include <core/extension/PaymentChannel/Payee.hpp>
#include <core/extension/Message/Buy.hpp>
#include <core/extension/Message/SignRefund.hpp>
#include <core/extension/Message/Payment.hpp>
#include <core/extension/Message/RequestFullPiece.hpp>

#include <QSet>
#include <QVector>

class SellerTorrentPlugin;
enum class PluginMode;

namespace libtorrent {
    struct disk_io_job;
}

#include <boost/shared_array.hpp>

/**
* HOW DO YOU FORWARD DECLARE A TEMPLATED TYPE?
namespace boost {
    class shared_array<char>;
}
*/

class SellerPeerPlugin : public PeerPlugin
{
public:

    /**
     * @brief State of peer.
     */
    class PeerState {

    public:

        /**
        // Enumeration of possible states
        // a peer can have when facing seller
        // and last message sent was
        // 1) welformed
        // 2) state compatible
        enum class LastValidAction {
            no_bitswapr_message_sent, // No message extended message beyond extended handshake has been sent
            mode_announced,
            sent_contract_invitation,
            requested_refund_signature,
            announced_ready,
            made_request,
            made_payment
        };
        */

        // Bad states
        enum class FailureMode {
            not_failed,

            // Time outs
            mode_message_time_out,
            contract_invitatio_time_out,
            refund_signature_request_time_out,
            announced_ready_time_out,

            payment_incorrectly_signed,

            /**
             * Contract issues
             */

            // Took to long for contract to appear on network
            contract_appearance_time_out,

            // Is this really a failure mode associated with a peer? is not peer's fault.
            //contract_confirmation_time_out,

            // Make more granular later:
            // fee to low
            // to large
            // etc
            contract_invalid
        };

        // Default constructor
        PeerState();

        // Constructor from members
        PeerState(//LastValidAction lastAction,
                  FailureMode failureMode,
                  const Buy & lastBuyReceived,
                  const SignRefund & lastSignRefundReceived,
                  const Payment & lastPaymentReceived,
                  const RequestFullPiece & lastRequestFullPieceReceived);

        // Getters and setters
        /**
        LastValidAction lastAction() const;
        void setLastAction(LastValidAction lastAction);
        */

        FailureMode failureMode() const;
        void setFailureMode(FailureMode failureMode);

        Buy lastBuyReceived() const;
        void setLastBuyReceived(const Buy & lastBuyReceived);

        SignRefund lastSignRefundReceived() const;
        void setLastSignRefundReceived(const SignRefund & lastSignRefundReceived);

        Payment lastPaymentReceived() const;
        void setLastPaymentReceived(const Payment & lastPaymentReceived);

        RequestFullPiece lastRequestFullPieceReceived() const;
        void setLastRequestFullPieceReceived(const RequestFullPiece &lastRequestFullPieceReceived);

    private:

        // Last valid action of peer
        //LastValidAction _lastAction;

        // How peer may have failed
        FailureMode _failureMode;

        // Message payloads received
        // Is updated so long as it is state compatible,
        // content may be invalid.
        Buy _lastBuyReceived;
        SignRefund _lastSignRefundReceived;
        Payment _lastPaymentReceived; // May be invalid, the valid payment is saved in Payee
        RequestFullPiece _lastRequestFullPieceReceived;
    };

    /**
     * @brief Enumeration of possible states the client.
     */
    enum class ClientState {

        // We have not sent any message after extended handshake
        no_bitswapr_message_sent,

        // We have sent seller mode message
        seller_mode_announced,

        // We have sent joining contract message
        joined_contract,

        // We ignored contract invitation: Why would we ever do this?
        ignored_contract_invitation,

        // We signed refund
        signed_refund,

        // We ignored signing refund invitation: Why would we ever do this?
        ignored_sign_refund_invitation,

        // Waiting for first request, after the ready message was sent
        awaiting_fullpiece_request_after_ready_announced,

        // Waiting for payment
        awaiting_payment,

        // Waiting for next request, after a payment was made
        awaiting_piece_request_after_payment,

        // We are doing async reading of a piece from disk,
        // which occurs when when a valid full piece request arrives
        reading_piece_from_disk,

        // We are trying to get the last payment received confirmed
        trying_to_claim_last_payment,

        //
        last_payment_confirmed
    };

    /**
     * @brief Plugin status, that is a snapshot of important information.
     */
    class Status : public PeerPlugin::Status {

    public:

        // Default constructor
        Status();

        // Constructor from members
        Status(PeerModeAnnounced peerModeAnnounced,
               BEPSupportStatus peerBEP10SupportStatus,
               BEPSupportStatus peerJoyStreamBEPSupportStatus,
               const PeerState & peerState,
               ClientState clientState,
               const Payee::Status & payeeStatus);

        virtual PluginMode pluginMode() const;

        // Getters and setters
        PeerState peerState() const;
        void setPeerState(const PeerState &peerState);

        ClientState clientState() const;
        void setClientState(ClientState clientState);

        Payee::Status payeeStatus() const;
        void setPayeeStatus(const Payee::Status &payeeStatus);

    private:

        // State of peer
        PeerState _peerState;

        // State of client
        ClientState _clientState;

        // Status of payee
        Payee::Status _payeeStatus;
    };

    // Constructor
    SellerPeerPlugin(SellerTorrentPlugin * torrentPlugin,
                     libtorrent::bt_peer_connection * connection,
                     bool scheduledForDeletingInNextTorrentPluginTick,
                     const Payee::Configuration & payeeConfiguration,
                     int numberOfPieces,
                     QLoggingCategory & category);

    // Destructor
    virtual ~SellerPeerPlugin();

    // Libtorrent callbacks
    char const * type() const;
    virtual void on_disconnect(libtorrent::error_code const & ec);
    virtual void on_connected();
    virtual bool on_extension_handshake(libtorrent::lazy_entry const & handshake);
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

    // Tries to broadcast most recent payment received
    void tryToClaimPayment();

    //
    //quint64 totalReceivedSinceStart() const;

    // Peer status
    Status status() const;

    // Getters and setters
    virtual PluginMode mode() const;
    virtual quint64 balance() const;

    PeerState peerState() const;
    void setPeerState(const PeerState &peerState);

    ClientState clientState() const;
    void setClientState(const ClientState &clientState);

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
    PeerState _peerState;

    // State of client
    ClientState _clientState;

    // Payee portion of payment channel
    Payee _payee;

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

#endif // SELLER_PEER_PLUGIN_HPP
