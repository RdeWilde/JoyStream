#ifndef PAYMENT_CHANNEL_VIEW_MODEL_HPP
#define PAYMENT_CHANNEL_VIEW_MODEL_HPP

class PeerPlugin;
class QStandardItemModel;
class QStandardItem;
class PeerPluginStatus;

class PaymentChannelViewModel
{
public:

    // Constructor
    PaymentChannelViewModel(PeerPlugin * peerPlugin, QStandardItemModel * paymentChannelsTableViewModel);

    void update(PeerPluginStatus status);
    void updateHost(const QString & host);
    void updateState();
    void updateBalance(int balance);
    void updateProgress(); // some representation of what it has given me?

private:

    // View model for payment channels table
    QStandardItem * paymentChannelsTableViewModel_;

    // paymentChannelsTableViewModel_ items
    QStandardItem * hostItem,
                  * stateItem,
                  * balanceItem,
                  * progressItem;
};

#endif // PAYMENT_CHANNEL_VIEW_MODEL_HPP
