#include "PayorViewModel.hpp"
#include "ChannelViewModel.hpp"

PayorViewModel::PayorViewModel(const Payor::Status & status)
    : _state(status.state())
    , _utxo(status.utxo())
    , _contractTxId(status.contractTxId())
    , _numberOfSignatures(status.numberOfSignatures()) {

    // Create view models for all channels
    QVector<Payor::Channel::Status> channelStatuses = status.channels();

    for(QVector<Payor::Channel::Status>::const_iterator
        i = channelStatuses.constBegin(),
        end = channelStatuses.constEnd();i != end;i++)
        _channelViewModels.append(new ChannelViewModel(*i));
}

PayorViewModel::~PayorViewModel() {

    for(QVector<ChannelViewModel *>::const_iterator
        i = _channelViewModels.constBegin(),
        end = _channelViewModels.constEnd();i != end;i++)
        delete (*i);
}

void PayorViewModel::update(const Payor::Status & status) {

    if(_state != status.state()) {
        _state = status.state();
        emit stateChanged(status.state());
    }

    if(_utxo != status.utxo()) {
        _utxo = status.utxo();
        emit utxoChanged(status.utxo());
    }

    if(_contractTxId != status.contractTxId()) {
        _contractTxId = status.contractTxId();
        emit contractTxIdChanged(_contractTxId);
    }

    if(_numberOfSignatures != status.numberOfSignatures()) {
        _numberOfSignatures = status.numberOfSignatures();
        emit numberOfSignaturesChanged(status.numberOfSignatures());
    }

    // Update each channel view model with new status
    QVector<Payor::Channel::Status> channelStatuses = status.channels();

    Q_ASSERT(_channelViewModels.size() != channelStatuses.size());

    for(int i = 0;i < _channelViewModels.size();i++)
        _channelViewModels[i]->update(channelStatuses[i]);
}

Payor::State PayorViewModel::state() const {
    return _state;
}

UnspentP2PKHOutput PayorViewModel::utxo() const {
    return _utxo;
}

TxId PayorViewModel::contractTxId() const {
    return _contractTxId;
}

quint32 PayorViewModel::numberOfSignatures() const {
    return _numberOfSignatures;
}

QVector<ChannelViewModel *> PayorViewModel::channelViewModels() const {
    return _channelViewModels;
}
