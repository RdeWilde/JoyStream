#ifndef SELLER_PEER_PLUGIN_HPP
#define SELLER_PEER_PLUGIN_HPP

#include "PeerPlugin.hpp"

#include "extension/PaymentChannel/Payee.hpp"

#include "Message/Buy.hpp"
#include "Message/SignRefund.hpp"
#include "Message/Payment.hpp"

//#include "extension/BitCoin/PublicKey.hpp"
//#include "extension/BitCoin/TxId.hpp"
//#include "extension/BitCoin/Signature.hpp"

#include <QSet>

class SellerTorrentPlugin;

class SellerPeerPlugin : public PeerPlugin
{
public:


    /**
     * @brief State of peer.
     */
    class PeerState {

    public:

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
            made_payment
        };

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
        PeerState(LastValidAction lastAction,
                  FailureMode failureMode,
                  const Buy & lastBuySent,
                  const SignRefund & lastSignRefundSent,
                  const Payment & lastPaymentSent);

        // Getters and setters
        LastValidAction lastAction() const;
        void setLastAction(LastValidAction lastAction);

        FailureMode failureMode() const;
        void setFailureMode(FailureMode failureMode);

        Buy lastBuySent() const;
        void setLastBuySent(const Buy & lastBuySent);

        SignRefund lastSignRefundSent() const;
        void setLastSignRefundSent(const SignRefund & lastSignRefundSent);

        Payment lastPaymentSent() const;
        void setLastPaymentSent(const Payment & lastPaymentSent);

    private:

        // Last valid action of peer
        LastValidAction _lastAction;

        // How peer may have failed
        FailureMode _failureMode;

        // Message payloads received
        Buy _lastBuySent;
        SignRefund _lastSignRefundSent;
        Payment _lastPaymentSent; // May be invalid, the valid payment is saved in Payee
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

        // We sent a piece message
        sent_piece
    };

    /**
     * @brief Plugin status, that is a snapshot of important information.
     */
    class Status {

    public:

        // Default constructor
        Status();

        // Constructor from members
        Status(const PeerState & peerState, ClientState clientState);

        // Getters and setters
        PeerState peerState() const;
        void setPeerState(const PeerState &peerState);

        ClientState clientState() const;
        void setClientState(ClientState clientState);

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

    // Getters and setters
    virtual PluginMode mode() const;

private:

    // Torrent level plugin
    SellerTorrentPlugin * _plugin;

    // State of peer
    PeerState _peerState;

    // State of client
    ClientState _clientState;

    // Payee portion of payment channel
    Payee _payee;

    /**
     * Request/Piece management
     */

    // Requests received but not serviced, in order
    QList<libtorrent::peer_request> _sPendingRequests;

    // Requests serviced
    QSet<libtorrent::peer_request> _serviced;

    // Processess message
    virtual void processObserve(const Observe * m);
    virtual void processBuy(const Buy * m);
    virtual void processSell(const Sell * m);
    virtual void processJoinContract(const JoinContract * m);
    virtual void processJoiningContract(const JoiningContract * m);
    virtual void processSignRefund(const SignRefund * m);
    virtual void processRefundSigned(const RefundSigned * m);
    virtual void processReady(const Ready * m);
    virtual void processPayment(const Payment * m);

    // Resets plugin in response to peer sending a mode message
    void peerModeReset();
};

#endif // SELLER_PEER_PLUGIN_HPP
