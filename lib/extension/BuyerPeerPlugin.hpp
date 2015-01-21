#ifndef BUYER_PEER_PLUGIN_HPP
#define BUYER_PEER_PLUGIN_HPP

#include "PeerPlugin.hpp"
#include "BitCoin/PublicKey.hpp"

/**
#include "BitCoin/Hash.hpp"
#include "BitCoin/Signature.hpp"
*/

class BuyerTorrentPlugin;

/**
 * @brief Buyer peer plugin
 */
class BuyerPeerPlugin : public PeerPlugin
{
public:

    /**
     * @brief Configuration of buyer peer plugin.
     */
    class Configuration {

    public:

        /**
         * @brief State of peer.
         */
        class Peer {

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

            // Constructor
            Peer();

            // Getters and setters
            LastValidAction lastAction() const;
            void setLastAction(const LastValidAction &lastAction);

            FailureMode failureMode() const;
            void setFailureMode(const FailureMode &failureMode);

            quint64 minPrice() const;
            void setMinPrice(const quint64 &minPrice);

            quint32 minLock() const;
            void setMinLock(const quint32 &minLock);

            PublicKey pK() const;
            void setPK(const PublicKey &pK);

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
         * @brief Enumeration of possible
         * states the client side of this
         * connection can have.
         */
        enum class Client {
            no_bitswapr_message_sent,
            buyer_mode_announced,
            invited_to_contract,
            asked_for_refund_signature,
            requested_piece,
            sent_payment
        };

        // Constructor
        Configuration();

        // Getters and setters
        Peer peerState() const;
        void setPeerState(const Peer &peerState);

        Client client() const;
        void setClient(const Client &client);

    private:

        Peer _peer;

        Client _client;
    };

    // Constructor
    BuyerPeerPlugin(BuyerTorrentPlugin * plugin, libtorrent::bt_peer_connection * connection, QLoggingCategory & category, const Configuration & configuration);

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
    virtual bool on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body) = 0;
    virtual void on_piece_pass(int index);
    virtual void on_piece_failed(int index);
    virtual void tick();
    virtual bool write_request(libtorrent::peer_request const & peerRequest);

    // Getters and setters
    Configuration configuration() const;
    virtual PluginMode mode() const;

private:

    BuyerTorrentPlugin * _plugin;

    Configuration _configuration;

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
    virtual void processEnd(const End * m);

    // Resets plugin in response to peer sending a mode message
    void peerModeReset();
};

#endif // BUYER_PEER_PLUGIN_HPP
