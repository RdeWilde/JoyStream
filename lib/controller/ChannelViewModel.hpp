#ifndef CHANNEL_VIEW_MODEL_HPP
#define CHANNEL_VIEW_MODEL_HPP

#include "extension/PaymentChannel/Payor.hpp"

#include <QObject>

class ChannelViewModel : public QObject {

    Q_OBJECT

public:

    // Constructor
    ChannelViewModel(QObject * parent,
                     quint32 index,
                     const Payor::Channel::Status & status);

    // Getters
    quint32 index() const;
    Payor::Channel::Status status() const;

public slots:

    // Update status
    void update(const Payor::Channel::Status & status);

signals:

    // Status change
    void stateChanged(Payor::Channel::State state);
    void fundsChanged(quint64 funds);
    void refundLockTimeChanged(quint32 refundLockTime);
    void priceChanged(quint64 price);
    void numberOfPaymentsMadeChanged(quint64 numberOfPaymentsMade);
    void balanceChanged(quint64 balance);

private:

    // Channel index in pyor
    quint32 _index;

    // Status of channel
    Payor::Channel::Status _status;
};

#endif // CHANNEL_VIEW_MODEL_HPP
