#ifndef CHANNEL_VIEW_MODEL_HPP
#define CHANNEL_VIEW_MODEL_HPP

#include "extension/PaymentChannel/Payor.hpp"

#include <libtorrent/socket_io.hpp> // libtorrent::tcp::endpoint

class ChannelViewModel : public QObject {

    Q_OBJECT

public:

    // Constructor
    ChannelViewModel(const Payor::Configuration & configuration);

    // Getters
    Payor::Channel::Configuration configuration() const;
    Payor::Channel::Status status() const;

public slots:

    // Update status
    void update(const Payor::Channel::Status & status);

signals:

    // Status signals
    void stateChanged(quint32 index, Payor::Channel::State state);
    void fundsChanged(quint32 index, quint64 funds);
    void refundLockTimeChanged(quint32 index, quint32 refundLockTime);
    void priceChanged(quint32 index, quint64 price);
    void numberOfPaymentsMadeChanged(quint32 index, quint64 numberOfPaymentsMade);
    void balanceChanged(quint32 index, quint64 balance);

private:

    // Configurations used to start channel
    Payor::Channel::Configuration _configuration;

    // Status of channel
    Payor::Channel::Status _status;
};

#endif // CHANNEL_VIEW_MODEL_HPP
