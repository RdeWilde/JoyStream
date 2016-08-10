/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 21 2015
 */

#ifndef UTXO_EVENT_VIEW_MODEL_HPP
#define UTXO_EVENT_VIEW_MODEL_HPP

#include <common/typesafeOutPoint.hpp>

namespace Wallet {
    class UtxoDestroyed;
    class UtxoCreated;
}

class QStandardItem;
class QStandardItemModel;
class QDateTime;
class BitcoinDisplaySettings;

/**
 * The polymorphism seen here likely should already have
 * been on the model level (Wallet::UtxoDestroyed, Wallet::UtxoCreated),
 * but lets fix that later.
 */

class UtxoEventViewModel {

    enum class Type {

        // creates utxo
        creation,

        // destroys/spends utxo
        destruction
    };

public:

    UtxoEventViewModel(QStandardItemModel * model,
                       const Wallet::UtxoDestroyed & event,
                       const BitcoinDisplaySettings * settings);

    UtxoEventViewModel(QStandardItemModel * model,
                       const Wallet::UtxoCreated & event,
                       const BitcoinDisplaySettings * settings);

    // Update view
    void updatConfirmations(const quint64 confirmations);

private:

    // BitCoin display settings
    const BitcoinDisplaySettings * _settings;

    // Setup view model
    void _setup(QStandardItemModel * model, const QDateTime & when, quint64 valye, quint64 confirmations);

    // What type of utxo event is this
    Type _eventType;

    // _eventType == creation => _id == outpoint of utxo created
    // _eventType == destruction => _id == outpoint of utxo spent
    Coin::typesafeOutPoint _id;

    // View model pointers
    QStandardItem * _when, // QDateTime
                  * _value, // quint64
                  * _confirmations; // quint64
};

#endif // UTXO_EVENT_VIEW_MODEL_HPP

