/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 21 2015
 */

#include <gui/UtxoEventViewModel.hpp>
#include <wallet/UtxoDestroyed.hpp>
#include <wallet/UtxoCreated.hpp>
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <QStandardItem>
#include <QStandardItemModel>
#include <QDateTime>

UtxoEventViewModel::UtxoEventViewModel(QStandardItemModel * model,
                                       const Wallet::UtxoDestroyed & event,
                                       const BitcoinDisplaySettings * settings)
    : _eventType(Type::destruction)
    , _id(event.inputInSpendingTx().previousOut) {

    _setup(model, event.seen(), event.oldUtxo().value, 0); // <--- better way to deal with confirmations
}

UtxoEventViewModel::UtxoEventViewModel(QStandardItemModel * model,
                                       const Wallet::UtxoCreated & event,
                                       const BitcoinDisplaySettings * settings)
    : _eventType(Type::creation)
    , _id(event.utxo().outPoint()){

    _setup(model, event.seen(), event.utxo().value(), 0); // <--- better way to deal with confirmations
}

void UtxoEventViewModel::_setup(QStandardItemModel * model, const QDateTime & when, quint64 value, quint64 confirmations) {

    // Add row to model
    QList<QStandardItem *> items;

    items << _when
          << _value
          << _confirmations;

    model->appendRow(items);

    // Align cell content
    _when->setData(Qt::AlignLeft, Qt::TextAlignmentRole);
    _value->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _confirmations->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Set start values
    _when->setText(when.toString());
    _value->setText(BitcoinRepresentation(value).toString(_settings));
    updatConfirmations(confirmations);
}

void UtxoEventViewModel::updatConfirmations(const quint64 confirmations) {
    _confirmations->setText(QString::number(confirmations));
}
