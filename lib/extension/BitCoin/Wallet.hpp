#ifndef WALLET_HPP
#define WALLET_HPP

#include "PrivateKey.hpp"
#include "OutPoint.hpp"

#include <QDateTime>

class Wallet
{
public:

    class Entry {

    public:

        /**
        * @brief Types of entries
        */
        enum class Type {
            ContractChange,
            Contract,
            Refund,
            Payment,
            Receive
        };

        // Default constructor
        Entry();

        // Constructor from members
        Entry(const OutPoint & point, Type type, const QString & description, const QDateTime & created, bool spent, const Hash & blockHash, quint32 blockHeight);

        // Getters and setters
        OutPoint point() const;
        void setPoint(const OutPoint &point);

        Type type() const;
        void setType(const Type &type);

        QString description() const;
        void setDescription(const QString &description);

        QDateTime created() const;
        void setCreated(const QDateTime &created);

        bool spent() const;
        void setSpent(bool spent);

        Hash blockHash() const;
        void setBlockHash(const Hash &blockHash);

        quint32 blockHeight() const;
        void setBlockHeight(const quint32 &blockHeight);

    private:

        // outpoint of entry
        OutPoint _point;

        // entry type
        Type _type;

        // entry description
        QString _description;

        // Time when entry was added to wallet
        QDateTime _created;

        // point is spent
        bool _spent;

        // Hash of bock to which _point belongs
        Hash _blockHash;

        // Height of block - use to compute #confirmations
        quint32 _blockHeight;
    };

private:

    // Wallet entries
    QMap<PrivateKey, QList<>> entries;
};

#endif // WALLET_HPP
