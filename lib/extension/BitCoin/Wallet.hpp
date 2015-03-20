#ifndef WALLET_HPP
#define WALLET_HPP

//#include "PrivateKey.hpp"
//#include "PrivateKey.hpp"
//#include "PublicKey.hpp"
#include "KeyPair.hpp"
#include "OutPoint.hpp"

#include <QDateTime>
#include <QFile>
#include <QMap>
#include <QVector>
#include <QSet>

class UnspentP2PKHOutput;

/**
 * @brief Naive Btc Micropayment wallet
 */
class Wallet
{
public:

    /**
     * @brief Different types of networks to which a wallet can belong
     */
    enum class Chain {
        mainnet,
        testnet3
    };

    /**
    * @brief Purpose for which key was generated
    */
    enum class Purpose {

        // Standard receive transaction, not associated with payment channel
        Receive,

        // All below are associated with payment channels

        /**
         * Buyer key purposes
         */

        // Change output of contract
        ContractChange,

        // Controls buyer portion of contract multisig output
        BuyerInContractOutput,

        // Final payment (change/refund) of a particular output of a contract
        ContractFinal,

        /**
         * Seller key purposes
         */

        // Controls sellers portion of contract multisig output
        SellerInContractOutput,

        // Payment of a particular output of a contract
        ContractPayment
    };

    /**
     * @brief Event associated with a transaction output controlled by a wallet key.
     */
    class TxOEvent {

    public:

        enum class Type {

            // Spend UTXO
            Send,

            // Receive UTXO
            Receive
        };

        // Default constructor
        TxOEvent();

        // Constructor from members
        TxOEvent(Type type,
               const OutPoint & outpoint,
               const quint64 value,
               const QDateTime & firstSeen,
               quint32 blockHeight);

        // Constructor from json dictionary
        TxOEvent(const QJsonObject & json);

        // Required for sorting
        bool operator<(const TxOEvent & o) const;

        // Save wallet as json dictionary
        QJsonObject toJson() const;

        // Getters and setters
        Type type() const;
        void setType(Type type);

        OutPoint outpoint() const;
        void setOutpoint(const OutPoint & outpoint);

        quint64 value() const;
        void setValue(quint64 value);

        QDateTime firstSeen() const;
        void setFirstSeen(const QDateTime & firstSeen);

        quint32 blockHeight() const;
        void setBlockHeight(quint32 blockHeight);

    private:

        // Type of TxO event
        // May not really be needed!!!!!!
        Type _type;

        // Send: spent utxo, Receive: received utxo
        OutPoint _outpoint;

        // Value of _outpoint
        quint64 _value;

        // Time event was first seen on wire
        QDateTime _firstSeen;

        // Height of block with transaction containing _outpoint
        // used to compute #confirmations
        quint32 _blockHeight;

        // !!!NOT USED AT PRESENT!!!
        // DO WE NEED THIS, AND SHOULD IT BE PART OF PERSISTENT WALLET STATE
        // Is set to true while the transaction is not in a block, i.e. only
        // observed, or the block is part of the longest known chain.
        bool _onMainChain;
    };

    /**
     * @brief Wallet entry
     */
    class Entry {

    public:

        // Defualt constructor for QMap
        Entry();

        // Constructor from members
        Entry(quint32 n,
            const KeyPair & keyPair,
            Purpose purpose,
            const QDateTime & generated,
            const QString & description,
            const QMap<OutPoint, TxOEvent> send,
            const QMap<OutPoint, TxOEvent> receive);

        // Constructor from json dictionary
        Entry(const QJsonObject & json);

        // Save wallet as json dictionary
        QJsonObject toJson() const;

        // Add TxO event
        //void addTxOEvent(const TxOEvent & txOEvent);

        void lockUtxo(const OutPoint & outPoint);

        bool isLocked(const OutPoint & outPoint) const;

        void unlockUtxo(const OutPoint & outPoint);

        // MAKE QSET LATER BY ADDING QHASH AND == TO TxOEvent as reuired
        QList<TxOEvent> getFreeUtxo() const;

        // Is given outpoint utxo for given key
        bool isUTxO(const OutPoint & outPoint) const;

        // Getters and setters
        quint32 n() const;
        void setN(quint32 n);

        KeyPair keyPair() const;
        void setKeyPair(const KeyPair & keyPair);

        Purpose purpose() const;
        void setPurpose(Purpose purpose);

        QDateTime generated() const;
        void setGenerated(const QDateTime & generated);

        QString description() const;
        void setDescription(const QString & description);

        QMap<OutPoint, TxOEvent> send() const;
        void setSend(const QMap<OutPoint, TxOEvent> &send);

        QMap<OutPoint, TxOEvent> receive() const;
        void setReceive(const QMap<OutPoint, TxOEvent> &receive);

    private:

        // Deterministic seed used for private key
        quint32 _n;

        // Key pair
        KeyPair _keyPair;

        // Purpose of key
        Purpose _purpose;

        // Time when key was added to wallet
        QDateTime _generated;

        // Description of key
        QString _description;

        // Transaction output events
        //QVector<TxOEvent> _txoEvents;

        // Send events, key is utxo spend in event
        QMap<OutPoint, TxOEvent> _send;

        // Receive events, key is utxo created in event
        QMap<OutPoint, TxOEvent> _receive;

        /**
         * Both sets below are deduced from _txoEvents,
         * they do not persist.
         */

        // Currently locked subset of _receive.
        // These will not be returned when
        // requesting utxo from wallet. Outputs are placed here when
        // a payment channel is being constructed which spends them, but
        // where the contract has not yet been broadcasted.
        QSet<OutPoint> _locked;
    };

    // Constructor from wallet file
    Wallet(const QString & file, bool autoSave);

    /**
     * Many of the routines below are semantically const, however
     * they cannot be in practices since they have to aquire
     * _mutex.
     */

    // Returns the private key for the given public key, or throws exception
    PrivateKey getSk(const PublicKey & pk);

    // Save wallet as json dictionary
    QJsonObject toJson(); // const

    // Number of keys in the wallet
    int numberOfKeysInWallet();  // const

    // Save to disk
    void save(); // const

    // Gets current block height
    static quint32 blockHeight();

    // Total balance in wallet with utxos with no less than given number of confirmations
    quint64 balance(quint32 confirmations); // const

    // Synchronizes wallet with blockchain
    void synchronize();

    // Kill later, only temporary
    QString toAddress(const PublicKey & pk) const;

    //Entry getAndLockEntry();
    UnspentP2PKHOutput getUtxo(quint64 minimalValue, quint32 minimalNumberOfConfirmations);

    // Add entry for a new receive address
    Entry addReceiveKey(const QString & description, Purpose purpose);

    // Generate a fresh set of keys
    QMap<PublicKey, Entry> generateNewKeys(quint8 numberOfKeys, Purpose purpose);

    // Getters and setters
    QMap<PublicKey, Entry> entries();  // const
    void setEntries(const QMap<PublicKey, Entry> &entries);
    Entry & entry(const PublicKey & publicKey);

    quint64 latestBlockHeight();  // const

    quint32 numberOfTransactions(); // const

private:

    // Seed used for wallet creation: (keep here?)
    qint64 _walletSeed;

    // Deterministic wallet gab limit
    int _gapLimit;

    // Number of keys generated since start of wallet,
    // is used to generate fresh keys.
    qint64 _keyCount;

    // Wallet entries
    QMap<PublicKey, Entry> _entries;

    // The chain to which wallet belongs
    Chain _chain;

    // Synchronizes wallet
    QMutex _mutex;

    // Wallet file name
    QString _walletFileName;

    // Wallet file
    QFile _walletFile;

    // Automatically save wallet to disk after every change
    bool _autoSave;

    // Latest known block height
    qint64 _latestBlockHeight;

    // Used by constructor to load wallet from dictionary, is not synchronized
    void fromJson(const QJsonObject & walletDictionary);

    /*
    // Add entry to _entries map
    void addEntry(const PublicKey & pk, const Entry & entry);

    // Add output to _outputs map of entry
    void addEntryOutput(const PublicKey & pk, const Entry::Output & output);
    */
};

#endif // WALLET_HPP
