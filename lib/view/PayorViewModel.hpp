#ifndef PAYOR_VIEW_MODEL_HPP
#define PAYOR_VIEW_MODEL_HPP

#include "ChannelViewModel.hpp"
#include "extension/PaymentChannel/Payor.hpp"

class BuyerTorrentPluginViewModel;

class PayorViewModel : public QObject
{
    Q_OBJECT
public:

    // Constructor from members
    PayorViewModel(quint32 numberOfSellers);


    // Update
    void update(const Payor::Status & status);

    // Getters
    QVector<ChannelViewModel *> channelViewModels() const;

signals:

    //void statusChanged(const Payor::Status & status);
    void contractTxIdChanged(const TxId & id);
    //void utxoChanged(const UnspentP2PKHOutput & utxo);

private:

    // Present transaction id of contract
    TxId _contractTxId;

    // View Models for all channels
    // ** We use pointers here since they are QObjects, which cannot be copied **
    QVector<ChannelViewModel *> _channelViewModels;

};

#endif // PAYOR_VIEW_MODEL_HPP
