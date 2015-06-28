#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/PrivateKey.hpp"
#include "extension/BitCoin/KeyPair.hpp"
#include "extension/BitCoin/Hash.hpp"
#include "extension/BitCoin/Signature.hpp"
#include "extension/BitCoin/OutPoint.hpp"

#include "Configuration/Configuration.hpp"

/**
 * Represents a single channel.
 */
class Channel {

public:

    class Configuration;

    /*
    // Forward declare nested classes

    class Configuration::Payor;
    class Configuration::Payee;

    */

    /**
     * @brief Enumeration of possible channel states.
     */
    enum class State {
        unassigned,
        assigned,
        refund_signed
    };

    // Default/Copy constructor and assignemtn operator needed to put in container.
    /*
    Channel();
    Channel(const Channel& slot);
    Channel & operator=(const Channel& rhs);
    */

    // Set all fields, e.g. loading from file
    /*
    Channel(quint32 index,
         const State &state,
         quint64 price,
         quint64 numberOfPaymentsMade,
         quint64 funds,
         const KeyPair &payorContractKeyPair,
         const PublicKey &payorFinalPk,
         const PublicKey &payeeContractPk,
         const PublicKey &payeeFinalPk,
         const Signature &refund,
         quint64 refundFee,
         quint64 paymentFee,
         quint32 refundLockTime);*/

    /**
     * Payment channel operations
     */

    // Refund transaction for slot
    //Refund refund(const Hash &contractHash) const;

    // Payment transaction for slot, based on current _numberOfPaymentsMade value
    //Payment payment(const Hash &contractHash) const;

    // Compute payor refund signature
    void computePayorRefundSignature(const Hash &contractHash) const;

    // Payment signature
    Signature paymentSignature(const Hash &contractHash) const;

    // Registers that a payment was made
    void paymentMade();

    QJsonObject json() const;

    // Getters and setters
    //void setPayorConfiguration(const Channel::Configuration::Payor & payorConfiguration);
    //void setPayeeConfiguration(const Channel::Configuration::Payee & payeeConfiguration);

private:

    // Configuration (remove later?)
    //Channel::Configuration _configuration;

    /**
    // Index in contract
    quint32 _index;

    // Slot state
    State _state;

    // Size of single payment
    quint64 _price;

    // Number of payments made
    quint64 _numberOfPaymentsMade;

    // Funds allocated to output
    quint64 _funds;

    // Controls payour output of multisig
    KeyPair _payorContractKeyPair;

    // Controls final payment to payor
    KeyPair _payorFinalKeyPair;

    // Controls payee output of multisig, received in joinin_contract.pk
    PublicKey _payeeContractPk;

    // Controls payee payments, received in sign_refund.pk
    PublicKey _payeeFinalPk;

    // Controls refund for payor
    Signature _payorRefundSignature;

    // Controls refund for payee
    Signature _payeeRefundSignature;

    // Fee used in refund transaction, is unlikely to vary across slots,
    quint64 _refundFee;

    // Fee used in payment transaction
    quint64 _paymentFee;

    // Lock time of refund, received in
    quint32 _refundLockTime;
    */
};

#endif // CHANNEL_HPP
