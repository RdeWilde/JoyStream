#ifndef CHANNEL_VIEW_MODEL_HPP
#define CHANNEL_VIEW_MODEL_HPP

#include "extension/PaymentChannel/Payor.hpp"

//#include <libtorrent/socket_io.hpp> // libtorrent::tcp::endpoint

class PayorViewModel;
class QStandardItem;

class ChannelViewModel {


public:

    // Constructor
    ChannelViewModel(PayorViewModel * model); //, libtorrent::tcp::endpoint & endPoint);

    // Update routine
    void updateHostItem(libtorrent::tcp::endpoint & endPoint);
    void updateUploadSpeedItem(quint64 bytesPrSecond);

    void update(const Payor::Channel::Status & status);
    void updateIndexItem(quint32 index);
    void updateStateItem(Payor::Channel::State state);
    void updateFundsItem(quint64 funds);
    void updateRefundLockTime(quint32 refundLockTime);
    void updatePriceItem(quint64 price);
    void updateNumberOfPaymentsMadeItem(quint64 numberOfPaymentsMade);
    void updateBalanceItem(quint64 balance);

private:

    QStandardItem * _hostItem,
                  * _indexItem,
                  * _stateItem,
                  * _fundsItem,
                  * _refundLockTimeItem,
                  * _priceItem,
                  * _numberOfPaymentsMadeItem,
                  * _balanceItem,
                  * _uploadSpeedItem;
};

#endif // CHANNEL_VIEW_MODEL_HPP
