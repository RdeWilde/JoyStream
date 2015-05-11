#ifndef PAYOR_VIEW_MODEL_HPP
#define PAYOR_VIEW_MODEL_HPP

#include "extension/PaymentChannel/Payor.hpp"

#include <QObject>

class ChannelViewModel;

class PayorViewModel : public QObject
{
    Q_OBJECT
public:

    // Constructor
    PayorViewModel(const Payor::Status & status);

    // Destructor
    ~PayorViewModel();

    // Update
    void update(const Payor::Status & status);

    // Getters
    Payor::State state() const;

    UnspentP2PKHOutput utxo() const;

    TxId contractTxId() const;

    quint32 numberOfSignatures() const;

    QVector<ChannelViewModel *> channelViewModels() const;

signals:

    // Status change
    void stateChanged(Payor::State state);
    void utxoChanged(const UnspentP2PKHOutput & utxo);
    void contractTxIdChanged(const TxId & id);
    void numberOfSignaturesChanged(quint32 numberOfSignatures);

private:

    // State of payor
    Payor::State _state;

    // Funding utxo
    UnspentP2PKHOutput _utxo;

    // Transaction id of contract
    TxId _contractTxId;

    // Number of valid signatures collected
    quint32 _numberOfSignatures;

    // View models for all channels
    // ** We use pointers here since they are QObjects, which cannot be copied **
    QVector<ChannelViewModel *> _channelViewModels;
};

#endif // PAYOR_VIEW_MODEL_HPP
