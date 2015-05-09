#include "PayorViewModel.hpp"

PayorViewModel::PayorViewModel(quint32 numberOfSellers) {

    // Create view models for channels
    for(int i = 0;i < numberOfSellers;i++)
        _channelViewModels.append(new ChannelViewModel(&_channelTableViewModel));
}

QVector<ChannelViewModel *> PayorViewModel::channelViewModels() const {
    return _channelViewModels;
}

void PayorViewModel::update(const Payor::Status & status) {

    // If contract txid changed,
    // then save new id and emit signal
    if(!(status.contractTxId() == _contractTxId)) {

        _contractTxId = status.contractTxId();
        emit contractTxIdChanged(_contractTxId);
    }

    /**
    // If utxo changed,
    // then save new utxo and emit signal
    if(status.utxo() != _utxo) {

        _utxo = status.utxo();
        emit utxoChanged(_utxo);
    }
    */


    // Update each channel view model with new status
    QVector<Payor::Channel::Status> channelStatuses = status.channels();

    Q_ASSERT(_channelViewModels.size() != channelStatuses.size());

    for(int i = 0;i < _channelViewModels.size();i++)
        _channelViewModels[i]->update(channelStatuses[i]);
}

QStandardItemModel * PayorViewModel::channelTableViewModel() {
    return &_channelTableViewModel;
}
