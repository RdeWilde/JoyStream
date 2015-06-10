#ifndef PAYEE_VIEW_MODEL_HPP
#define PAYEE_VIEW_MODEL_HPP

#include "extension/PaymentChannel/Payee.hpp"

#include <QObject>

class PayeeViewModel : public QObject
{

    Q_OBJECT

public:

    // Constructor
    PayeeViewModel(QObject * parent,
                   const Payee::Status & status);

    // Update
    void update(const Payee::Status & status);

    // Getters
    Payee::Status status() const;

signals:

    // Update
    void stateChanged(Payee::State state);
    void contractOutPointChanged(const OutPoint & o);
    void numberOfPaymentsMadeChanged(quint64 numberOfPaymentsMade);
    void lockTimeChanged(quint32 lockTime);
    void priceChanged(quint64 price);
    void fundsChanged(quint64 funds);
    void balanceChanged(quint64 balance);

private:

    // Payee status
    Payee::Status _status;
};

#endif // PAYEE_VIEW_MODEL_HPP
