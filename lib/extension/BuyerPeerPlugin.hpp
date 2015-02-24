#ifndef BUYER_PEER_PLUGIN_HPP
#define BUYER_PEER_PLUGIN_HPP

#include "PeerPlugin.hpp"
#include "PluginMode.hpp"

#include "BitCoin/Hash.hpp"
#include "BitCoin/PublicKey.hpp"

class BuyerTorrentPlugin;

/**
 * @brief Buyer peer plugin
 */
class BuyerPeerPlugin : public PeerPlugin
{
public:

    /**
     * @brief State of peer.
     */
    class PeerState {

    public:

        // Enumeration of possible states
        // a peer can have when facing buyer
        // and last message sent was
        // 1) welformed
        // 2) state compatible
        enum class LastValidAction {
            no_bitswapr_message_sent, // No message extended message beyond extended handshake has been sent
            mode_announced,
            joined_contract,
            signed_refund,
            sent_valid_piece
        };

        // Bad states
        enum class FailureMode {
            not_failed,
            mode_message_time_out,
            join_contract_time_out,
            refund_signature_time_out,
            refund_incorrectly_signed,
            sent_invalid_piece, // wrong piece, or integrity not intact
        };

        // Default constructor
        PeerState();

        // Constructor from members
        PeerState(LastValidAction lastAction,
                  FailureMode failureMode,
                  quint64 minPrice,
                  quint32 _minLock,
                  const PublicKey & pK);

        // Getters and setters
        LastValidAction lastAction() const;
        void setLastAction(LastValidAction lastAction);

        FailureMode failureMode() const;
        void setFailureMode(FailureMode failureMode);

        quint64 minPrice() const;
        void setMinPrice(quint64 minPrice);

        quint32 minLock() const;
        void setMinLock(quint32 minLock);

        PublicKey pK() const;
        void setPK(const PublicKey & pK);

    private:

        // Last valid action of peer
        LastValidAction _lastAction;

        // How peer may have failed
        FailureMode _failureMode;

        // seller mode fields
        quint64 _minPrice;

        quint32 _minLock;

        // joining contract fields
        PublicKey _pK;
    };

    /**
     * @brief Enumeration of possible states the client.
     */
    enum class ClientState {

        no_bitswapr_message_sent,

        buyer_mode_announced,

        invited_to_contract,

        asked_for_refund_signature,

        requested_piece,

        sent_payment
    };

    /**
     * @brief Plugin status, that is a snapshot
     * of important information.
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
    };

    /**
     * @brief Configuration of buyer peer plugin.
     */
    class Configuration : public PeerPlugin::Configuration {

    public:

        // Constructor
        Configuration();

        // Constructor from members
        Configuration(const PeerState & peerState, ClientState clientState);

        // Getters and setters
        PeerState peerState() const;
        void setPeerState(const PeerState & peerState);

        ClientState clientState() const;
        void setClientState(ClientState clientState);

    private:

        // State of peer
        PeerState _peerState;

        // State of client
        ClientState _clientState;
    };

    // Constructor
    BuyerPeerPlugin(BuyerTorrentPlugin * plugin,
                    libtorrent::bt_peer_connection * connection,
                    const Configuration & configuration,
                    QLoggingCategory & category);

    // Destructor
    virtual ~BuyerPeerPlugin();

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

    Status status() const;

    // Getters and setters
    PeerState peerState() const;
    void setPeerState(const PeerState & peerState);

    ClientState clientState() const;
    void setClientState(ClientState clientState);

    virtual PluginMode mode() const;

private:

    // Torrent level plugin
    BuyerTorrentPlugin * _plugin;

    // State of peer
    PeerState _peerState;

    // State of client
    ClientState _clientState;

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
    //virtual void processEnd(const End * m);

    // Resets plugin in response to peer sending a mode message
    void peerModeReset();
};

#endif // BUYER_PEER_PLUGIN_HPP
