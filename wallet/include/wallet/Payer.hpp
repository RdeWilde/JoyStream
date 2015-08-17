/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_PAYER_HPP
#define WALLET_PAYER_HPP

#include <common/TransactionId.hpp>

#include <QDateTime>

class QSqlQuery;
class QSqlDatabase;

namespace Wallet {
namespace Payer {

    /**
     * @brief States of a payer.
     */
    enum class State {

        // Waiting to get PayeeConfiguration for all slots
        waiting_for_full_set_of_sellers,

        // Waiting to get refund signatures from all sellers
        waiting_for_full_set_of_refund_signatures,

        // Contract has been broadcasted, and at least one contract output
        // remains unspent and with sufficient balanc
        can_pay,

        // When
        all_contract_outputs_spent
    };

    class Record
    {
    public:

        // State to encoding bijection
        static quint8 encodeState(State state);
        static State decodeState(quint8 state);

        // Constructor from members
        Record(quint64 id, const Coin::TransactionId & contractTransactionId, State state, const QDateTime & created, const QString & description);

        // Constructor from record
        // Payer(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

        // Getters and setters
        quint64 id() const;
        void setId(quint64 id);

        Coin::TransactionId contractTransactionId() const;
        void setContractTransactionId(const Coin::TransactionId & contractTransactionId);

        quint64 contractFee() const;
        void setContractFee(quint64 contractFee);

        State state() const;
        void setState(State state);

        QDateTime created() const;
        void setCreated(const QDateTime & created);

        QString description() const;
        void setDescription(const QString & description);

    private:

        // Id of payer
        quint64 _id;

        // Transaction id of contract
        Coin::TransactionId _contractTransactionId;

        // Number of satoshies used as fee in contract
        quint64 _contractFee;

        // State of payer
        State _state;

        // When payer was created
        QDateTime _created;

        // Description of payer
        QString _description;
    };

    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unboundedInsertQuery(QSqlDatabase db);

}
}

#endif // WALLET_PAYER_HPP
