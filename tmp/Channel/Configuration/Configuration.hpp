#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "extension/BitCoin/KeyPair.hpp"

#include <QtGlobal>

class Channel;

class Channel::Configuration {

public:

    /**
     * @brief Payor provided values in channel,
     * as through wire message.
     */
    class Payor {

    public:

        // Default constructor
        Payor();

        // Member constructor
        Payor(quint64 funds, const KeyPair & contractKeyPair, const KeyPair & finalKeyPair);

        // Copy constructor
        Payor(const Channel::Configuration::Payor & o);

        // Assignment operator
        Payor & Payor::operator=(const Payor & o);

        // Getters and setters
        quint64 funds() const;
        void setFunds(quint64 funds);

        KeyPair contractKeyPair() const;
        void setContractKeyPair(const KeyPair &contractKeyPair);

        KeyPair finalKeyPair() const;
        void setFinalKeyPair(const KeyPair &finalKeyPair);

    private:

        // Funds allocated to output
        quint64 _funds;

        // Controls payour output of multisig
        KeyPair _contractKeyPair;

        // Controls final payment to payor
        KeyPair _finalKeyPair;
    };

    /**
     * @brief Payee provided values in channel,
     * as through wire message.
     */
    class Payee {

    public:

        // Default constructor
        Payee();

        // Constructor based on members
        Payee(quint64 price, const PublicKey & contractPk, const PublicKey & finalPk, quint32 refundLockTime);

        // Copy constructor
        Payee(const Channel::Configuration::Payee & o);

        // Assignment operator
        Payee & Payee::operator=(const Payee & o);

        // Getters and setters
        quint64 price() const;
        void setPrice(quint64 price);

        PublicKey contractPk() const;
        void setContractPk(const PublicKey & contractPk);

        PublicKey finalPk() const;
        void setFinalPk(const PublicKey & finalPk);

        quint32 refundLockTime() const;
        void setRefundLockTime(quint32 refundLockTime);

    private:

        // Size of single payment
        quint64 _price;

        // Controls payee output of multisig, received in joinin_contract.pk
        PublicKey _contractPk;

        // Controls payee payments, received in sign_refund.pk
        PublicKey _finalPk;

        // Refund lock time
        quint32 _refundLockTime;
    };

    Channel::Configuration();

    // Getters and setters
    Channel::Configuration::Payor payorConfiguration() const;
    void setPayorConfiguration(const Channel::Configuration::Payor & payorConfiguration);

    Channel::Configuration::Payee payeeConfiguration() const;
    void setPayeeConfiguration(const Channel::Configuration::Payee & payeeConfiguration);

private:

    // Index in contract
    quint32 _index;

    // Slot state
    Channel::State _state;

    // Payor configurations
    Channel::Configuration::Payor _payorConfiguration;

    // Payee configuration
    Channel::Configuration::Payee _payeeConfiguration;
};

#endif // CONFIGURATION_H
