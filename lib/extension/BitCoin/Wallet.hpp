#ifndef WALLET_HPP
#define WALLET_HPP

#include "PrivateKey.hpp"
#include "OutPoint.hpp"

#include <QDateTime>
#include <QFile>
#include <QMap>

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
     * @brief Wallet entry
     */
    class KeyInformation {

    public:

        /**
         * @brief Private key source
         */
        enum class Source {
            Generated,
            Imported
        };

        /**
         * @brief
         */
        class Output {

        public:

            /**
            * @brief Types of output
            */
            enum class Type {

                // Change output of contract
                ContractChange,

                // Change of a particular output of a contract
                ContractOutputChange,

                // Refund of a particular output of a contract
                ContractOutputRefund,

                // Payment of a particular output of a contract
                Payment,

                // Standard receive transaction, not associated with payment channel
                Receive
            };

            /**
             * @brief State of output
             */
            enum class State {

                // Whether output is reserved for
                // some spending purpose.

                Free,
                Locked
            };

            // Default constructor
            Output();

            // Constructor from members
            Output(const OutPoint & point,
                   const QDateTime & added,
                   const QString & description,
                   Type type,
                   State state,
                   quint64 value,
                   const Hash & blockHash,
                   quint32 blockHeight,
                   bool spent);

            // Constructor from json dictionary
            Output(const QJsonObject & json);

            // Used for comparison function to put Output in QMap
            bool operator<(const Output & rhs);

            // Save wallet as json dictionary
            QJsonObject toJson() const;

            // Getters and setters
            OutPoint point() const;
            void setPoint(const OutPoint & point);

            QDateTime added() const;
            void setAdded(const QDateTime & added);

            QString description() const;
            void setDescription(const QString & description);

            Type type() const;
            void setType(Type type);

            State state() const;
            void setState(State state);

            quint64 value() const;
            void setValue(quint64 value);

            Hash blockHash() const;
            void setBlockHash(const Hash & blockHash);

            quint32 blockHeight() const;
            void setBlockHeight(quint32 blockHeight);

            bool spent() const;
            void setSpent(bool spent);

        private:

            // Outpoint
            OutPoint _point;

            // Time when output was added to wallet
            QDateTime _added;

            // Description of output
            QString _description;

            // Semantic type of output
            Type _type;

            // State of outpoint in wallet
            State _state;

            // Value of output
            quint64 _value;

            // Hash of bock to which _point belongs
            Hash _blockHash;

            // Height of block - use to compute #confirmations
            quint32 _blockHeight;

            // point is spent
            bool _spent;
        };

        // Defualt constructor for QMap
        KeyInformation();

        // Constructor from members
        KeyInformation(const QDateTime & added,
              const QString & description,
              const QMap<OutPoint, Output> & outputs);

        // Constructor from json dictionary
        KeyInformation(const QJsonObject & json);

        // Save wallet as json dictionary
        QJsonObject toJson() const;

        // Getters and setters
        QDateTime added() const;
        void setAdded(const QDateTime & added);

        QString description() const;
        void setDescription(const QString & description);

        QMap<OutPoint, Output> outputs() const;
        void setOutputs(const QMap<OutPoint, Output> & outputs);

    private:

        // Time when key was added to wallet
        QDateTime _added;

        // Description of key
        QString _description;

        // All outputs associated with given key
        QMap<OutPoint, Output> _outputs;
    };

    // Constructor from wallet file
    Wallet(const QString & file, bool autoSave);

    /**
     * Many of the routines below are semantically const, however
     * they cannot be in practices since they have to aquire
     * _mutex.
     */

    // Save wallet as json dictionary
    QJsonObject toJson(); // const

    // Save to disk
    void save(); // const

    // Gets current block height
    static quint32 blockHeight();

    // Total balance in wallet with utxos with no less than given number of confirmations
    quint64 balance(quint32 confirmations); // const

    // Synchronizes wallet with blockchain
    void synchronize();

    //Entry getAndLockEntry();

private:

    // Wallet entries
    QMap<PrivateKey, KeyInformation> _entries;

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
};

#endif // WALLET_HPP
