#ifndef PAYOR_HPP
#define PAYOR_HPP

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/PrivateKey.hpp"
#include "extension/BitCoin/KeyPair.hpp"
#include "extension/BitCoin/Hash.hpp"
#include "extension/BitCoin/Signature.hpp"
#include "extension/BitCoin/OutPoint.hpp"

#include "Channel.hpp"

#include <QVector>

//class Refund;
//class Payment;
//class Contract;

/**
 * 1-to-N payment channel from payor perspective, using design in CBEP.
 * https://github.com/bedeho/CBEP
 *
 */
class Payor
{
public:

    /**
     * @brief State of a payor.
     */
    enum class State {

        // Waiting to get PayeeConfiguration for all slots
        waiting_for_full_set_of_sellers,

        // Waiting to get refund signatures from all sellers
        waiting_for_full_set_of_refund_signatures,

        // Payments are being made
        paying,

        // No more payments are being made, but some refunds are still spendable.
        waiting_for_all_refunds_to_be_spendable,

        // Ally refunds are spent or double spent, hence we are done
        done
    };


    // Default constructor
    Payor();

    // Constructor based on members
    Payor(const QSet<Channel::PayorConfiguration> & configurations, const OutPoint& fundingOutput, const KeyPair& fundingOutputKeyPair);

    // Finds an unassigned slot
    // ========================
    // If one is found then the
    // given payee slot configurations are saved in slot,
    // and if this was last unassigned slot, then payor state is switched.
    quint32 assignUnassignedSlot(const Channel::PayeeConfiguration & configuration);

    // Resets slot state to unassigned
    // ===============================
    // If payor has state State::waiting_for_full_set_of_refund_signatures, then it is switched
    // back to State::waiting_for_full_set_of_sellers, and all all other slot state
    // set to Slot::State::assigned, even if they had Slot::State::refund_signed, since
    // new signatures are now required.
    void unassignSlot(quint32 index);

    // Returns validity of signature for given slot
    // ============================================
    // If payor is collecting signatures
    // and if signature is valid, then
    // 1) saves signature
    // 2) updates slot state refund_assigned
    // 3) updates payor state to all_signed, if all all are now signed.
    bool processRefundSignature(quint32 index, const Signature & signature);

    // Returns the payment signature for the present payment increment of given slot
    // ============================================
    Signature getPresentPaymentSignature(quint32 index) const;

    // Attempts to claim refund for given slot
    // Returns false iff (time lock has not experied on refund or output has been double spent)
    bool claimRefund(quint32 index) const;

    // Checks if output is spent
    bool spent(quint32 index) const;

    // Getters and setters
    OutPoint fundingOutput() const;
    void setFundingOutput(const OutPoint &fundingOutput);

    quint32 numberOfSignedSlots() const;
    void setNumberOfSignedSlots(const quint32 &numberOfSignedSlots);

private:

    // Payor state
    State _state;

    // Contract outputs
    QVector<Channel> _channels;

    // Unspent output funding channel
    OutPoint _fundingOutput;

    // Controls output funding channel
    KeyPair _fundingOutputKeyPair;

    // Controls change output in contract
    KeyPair _changeOutputKeyPair;

    // Change amount sent back to payor,
    // this value, together with the _funds in all the slots
    // determines how much is paid in contract fee implicitly.
    quint64 _changeValue;

    /**
     * Contract:
     * ==========================
     * Is recomputed every time a full set of sellers is established,
     * and is cleared whenever a signature failed.
     */

    //Contract _contract;
    Hash _contractHash;
    quint32 _numberOfSignedSlots;

};

#endif // PAYOR_HPP
