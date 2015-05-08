#ifndef PAYOR_VIEW_MODEL_HPP
#define PAYOR_VIEW_MODEL_HPP

#include "ChannelViewModel.hpp"
#include "extension/PaymentChannel/Payor.hpp"

#include <QVector>
#include <QStandardItemModel>

class BuyerTorrentPluginViewModel;

class PayorViewModel
{
public:

    static const char * columnTitles[];
    static const int numberOfColumns;

    // Constructor from members
    PayorViewModel(const TxId & contractTxId,
                   const UnspentP2PKHOutput & utxo,
                   const QVector<ChannelViewModel> & channelViewModels);

    // Update
    void update(const Payor::Status & status);

    // Getters and setters
    QStandardItemModel * channelTableViewModel();

signals:

    //void statusChanged(const Payor::Status & status);
    void contractTxIdChanged(const TxId & id);
    void utxoChanged(const UnspentP2PKHOutput & utxo);

private:

    // Present transaction id of contract
    TxId _contractTxId;

    // Present utxo funding contract
    UnspentP2PKHOutput _utxo;

    // View Models for all channels
    QVector<ChannelViewModel> _channelViewModels;

    // Buyer peer plugin table view model
    QStandardItemModel _channelTableViewModel;

};

#endif // PAYOR_VIEW_MODEL_HPP
