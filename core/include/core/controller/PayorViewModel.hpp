/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PAYOR_VIEW_MODEL_HPP
#define PAYOR_VIEW_MODEL_HPP

#include <core/extension/PaymentChannel/Payor.hpp>

#include <QObject>

class ChannelViewModel;

class PayorViewModel : public QObject
{
    Q_OBJECT
public:

    // Constructor
    PayorViewModel(QObject * parent, const Payor::Status & status);

    // Destructor
    //~PayorViewModel();

    // Update
    void update(const Payor::Status & status);

    // Getters
    Payor::State state() const;

    Coin::UnspentP2PKHOutput utxo() const;

    Coin::TransactionId contractTxId() const;

    quint32 numberOfSignatures() const;

    QVector<ChannelViewModel *> channelViewModels() const;

signals:

    // Status change
    void stateChanged(Payor::State state);
    void contractTxIdChanged(const Coin::TransactionId & id);
    void utxoChanged(const Coin::UnspentP2PKHOutput & utxo);
    void numberOfSignaturesChanged(quint32 numberOfSignatures);

private:

    // State of payor
    Payor::State _state;

    // Funding utxo
    Coin::UnspentP2PKHOutput _utxo;

    // Transaction id of contract
    Coin::TransactionId _contractTxId;

    // Number of valid signatures collected
    quint32 _numberOfSignatures;

    // View models for all channels
    // ** We use pointers here since they are QObjects, which cannot be copied **
    QVector<ChannelViewModel *> _channelViewModels;
};

#endif // PAYOR_VIEW_MODEL_HPP
