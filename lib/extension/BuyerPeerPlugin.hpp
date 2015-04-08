#ifndef BUYER_PEER_PLUGIN_HPP
#define BUYER_PEER_PLUGIN_HPP

#include "PeerPlugin.hpp"
#include "PluginMode.hpp"

#include "BitCoin/TxId.hpp"
#include "BitCoin/PublicKey.hpp"
#include "Utilities.hpp" // uint qHash(const libtorrent::peer_request & request);

#include <QSet>

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
                  const PublicKey & contractPk,
                  const PublicKey & finalPk);

        // Getters and setters
        LastValidAction lastAction() const;
        void setLastAction(LastValidAction lastAction);

        FailureMode failureMode() const;
        void setFailureMode(FailureMode failureMode);

        quint64 minPrice() const;
        void setMinPrice(quint64 minPrice);

        quint32 minLock() const;
        void setMinLock(quint32 minLock);

        PublicKey contractPk() const;
        void setContractPk(const PublicKey & contractPk);

        PublicKey finalPk() const;
        void setFinalPk(const PublicKey & finalPk);

    private:

        // Last valid action of peer
        LastValidAction _lastAction;

        // How peer may have failed
        FailureMode _failureMode;

        // seller mode fields
        quint64 _minPrice;
        quint32 _minLock;

        // joining contract fields
        PublicKey _contractPk;
        PublicKey _finalPk;
    };

    /**
     * @brief Enumeration of possible states the client.
     */
    enum class ClientState {

        // We have not sent any message after extended handshake
        no_bitswapr_message_sent,

        // We have sent buyer mode message
        buyer_mode_announced,

        // We have sent join_contraact message
        invited_to_contract,

        // We ignored joining_contract message because the contract was full
        ignored_join_contract_from_peer,

        // We have sent sign_refund message
        asked_for_refund_signature,

        // Postponed sendining ready message since not all signatures were ready
        received_valid_refund_signature_and_waiting_for_others,

        // Sent ready message
        // NOT REALLY NEEDED, WE NEVER WAIT FOR ANYTHIN AFTER THIS
        //announced_ready,

        // At least one request message has been sent, for which no piece message
        // has yet been returned
        waiting_for_requests_to_be_serviced,

        // All sent requests have been serviced by peer
        needs_to_be_assigned_piece,

        // We have received piece messages for all blocks in currently assigned piece,
        // hence we are waiting for libtorrent to fire on_piece_pass() or on_piece_failed()
        waiting_for_libtorrent_to_validate_piece

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
        Configuration(const ExtendedMessageIdMapping & clientMapping,
                      const ExtendedMessageIdMapping & peerMapping,
                      BEPSupportStatus peerBEP10SupportStatus,
                      BEPSupportStatus peerBitSwaprBEPSupportStatus,
                      const PeerState & peerState,
                      ClientState clientState,
                      quint32 payorSlot);

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

        // Payor slot: payment channel output slot
        quint32 _payorSlot;
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

    quint32 refillPipeline();

    Status status() const;

    // Getters and setters
    PeerState peerState() const;
    void setPeerState(const PeerState & peerState);

    ClientState clientState() const;
    void setClientState(ClientState clientState);

    quint32 payorSlot() const;
    void setPayorSlot(quint32 payorSlot);

    //bool assignedPiece() const;
    //void setAssignedPiece(bool assignedPiece);

    int indexOfAssignedPiece() const;
    void setIndexOfAssignedPiece(int indexOfAssignedPiece);

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

    QSet<libtorrent::peer_request> unservicedRequests() const;
    void setUnservicedRequests(const QSet<libtorrent::peer_request> &unservicedRequests);

    QDateTime whenLastRequestServiced() const;
    void setWhenLastRequestServiced(const QDateTime &whenLastRequestServiced);

    QSet<int> downloadedPieces() const;
    void setDownloadedPieces(const QSet<int> &downloadedPieces);

    virtual PluginMode mode() const;

private:

    // Torrent level plugin
    BuyerTorrentPlugin * _plugin;

    // State of peer
    PeerState _peerState;

    // State of client
    ClientState _clientState;

    // Payor slot: payment channel output slot
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

    // Piece index for which this
    int _indexOfAssignedPiece;

    // Byte length of presently assigned piece
    int _pieceSize;

    // Byte length of blocks
    int _blockSize;

    // The number of blocks in the presently assigned piece
    int _numberOfBlocksInPiece;

    // The number of blocks requested
    int _numberOfBlocksRequested;

    // The number of blocks which were requested and were subsequently received in a piece message
    int _numberOfBlocksReceived;

    // Block indexes for block requests which have been issued, but not responded to
    QSet<libtorrent::peer_request> _unservicedRequests;

    // Time when last request was serviced
    QDateTime _whenLastRequestServiced;

    // Piece indexes, in download order, of
    // all valid pieces downloaded from seller peer during this session
    QSet<int> _downloadedPieces;

    /**
     * In the future the next two pipeline variables have to be
     * dynamically adjusted based on connection latency and
     * downstream bandwidth.
     */

    // The maximum number of requests which can be pipelined
    const static quint32 _requestPipelineLength = 10;

    // Refill pipline when it falls below this bound
    const static quint32 _requestPipelineRefillBound = 5;

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
