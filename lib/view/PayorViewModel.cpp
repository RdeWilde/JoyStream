#include "PayorViewModel.hpp"


const char * PayorViewModel::columnTitles[] = {"Host", "Index", "State", "Funds", "Lock", "Price", "#Payments", "Balance", "Speed"};
const int PayorViewModel::numberOfColumns = sizeof(PayorViewModel::columnTitles)/sizeof(char *);

PayorViewModel::PayorViewModel(const TxId & contractTxId,
                               const UnspentP2PKHOutput & utxo,
                               const QVector<ChannelViewModel> & channelViewModels)
    : _contractTxId(contractTxId)
    , _utxo(utxo)
    , _channelViewModels(channelViewModels) {

    // Add columns to model view model
    for(int i = 0;i < SellerTorrentPluginViewModel::numberOfColumns;i++)
        _channelTableViewModel.setHorizontalHeaderItem(i, new QStandardItem(PayorViewModel::columnTitles[i]));
}

void PayorViewModel::update(const Payor::Status & status) {

    // If contract txid changed,
    // then save new id and emit signal
    if(status.contractTxId() != _contractTxId) {

        _contractTxId = status.contractTxId();
        emit contractTxIdChanged(_contractTxId);
    }

    // If utxo changed,
    // then save new utxo and emit signal
    if(status.utxo() != _utxo) {

        _utxo = status.utxo();
        emit utxoChanged(_utxo);
    }

    // Update each channel view model with new status
    QVector<Payor::Channel::Status> channelStatuses = status.channels();

    Q_ASSERT(_channelViewModels.size() != channelStatuses.size());

    for(int i = 0;i < channelStatuses.size();i++)
        _channelViewModels[i].update(channelStatuses[i]);
}

QStandardItemModel * PayorViewModel::channelTableViewModel() {
    return &_channelTableViewModel;
}
