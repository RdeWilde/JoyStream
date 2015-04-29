#ifndef BUYER_PEER_PLUGIN_HPP
#define BUYER_PEER_PLUGIN_HPP

#include "PeerPlugin.hpp"
#include "PluginMode.hpp"

#include "BitCoin/TxId.hpp"
#include "BitCoin/PublicKey.hpp"
#include "Utilities.hpp" // uint qHash(const libtorrent::peer_request & request);
#include "Message/Sell.hpp"
#include "Message/JoiningContract.hpp"

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

        /**
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
        */

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
        PeerState(//LastValidAction lastAction,
                  FailureMode failureMode,
                  const Sell & lastSellReceived,
                  const JoiningContract & lastJoiningContractReceived);

        // Getters and setters

        //LastValidAction lastAction() const;
        //void setLastAction(LastValidAction lastAction);

        FailureMode failureMode() const;
        void setFailureMode(FailureMode failureMode);

        Sell lastSellReceived() const;
        void setLastSellReceived(const Sell &lastSellReceived);

        JoiningContract lastJoiningContractReceived() const;
        void setLastJoiningContractReceived(const JoiningContract &lastJoiningContractReceived);

    private:

        // Last valid action of peer
        //LastValidAction _lastAction;

        // How peer may have failed
        FailureMode _failureMode;

        // Last seller message
        Sell _lastSellReceived;

        // Last joining contract message
        JoiningContract _lastJoiningContractReceived;
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
        //waiting_for_requests_to_be_serviced,

        // Not been assigned piece
        needs_to_be_assigned_piece,

        // We have requested a full piece, and we are waiting for it to arrive
        waiting_for_full_piece,

        // We are waiting for libtorrent to fire on_piece_pass() or on_piece_failed()
        // on a full piece which was recently received
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
        Status(const PeerState & peerState,
               ClientState clientState,
               quint32 payorSlot,
               int indexOfAssignedPiece,
               const QList<int> & downloadedValidPieces);

        // Getters and setters
        PeerState peerState() const;
        void setPeerState(const PeerState & peerState);

        ClientState clientState() const;
        void setClientState(ClientState clientState);

        quint32 payorSlot() const;
        void setPayorSlot(const quint32 payorSlot);

        int indexOfAssignedPiece() const;
        void setIndexOfAssignedPiece(int indexOfAssignedPiece);

        QList<int> downloadedValidPieces() const;
        void setDownloadedValidPieces(const QList<int> &downloadedValidPieces);

    private:

        // State of peer
        PeerState _peerState;

        // State of client
        ClientState _clientState;

        // Peer plugin position in Payor
        quint32 _payorSlot;

        // Index of a piece assigned to this peer
        int _indexOfAssignedPiece;

        // Piece indexes, in download order, of
        // all valid pieces downloaded from seller peer during this session
        QList<int> _downloadedValidPieces;
    };

    // Constructor
    BuyerPeerPlugin(BuyerTorrentPlugin * plugin,
                    libtorrent::bt_peer_connection * connection,
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

    //quint32 refillPipeline();

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

    //int pieceSize() const;
    //void setPieceSize(int pieceSize);

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

    QList<int> downloadedPieces() const;
    void setDownloadedPieces(const QList<int> &downloadedPieces);

    void addDownloadedPiece(int index);

    virtual PluginMode mode() const;

private:

    // Torrent level plugin
    // Should we use a boost::shared_ptr instead since object lifetime is managed by it?
    BuyerTorrentPlugin * _plugin;

    // State of peer
    PeerState _peerState;

    // State of client
    ClientState _clientState;

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

#endif // BUYER_PEER_PLUGIN_HPP
