/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 26 2016
 */

#ifndef BLOCKCYPHER_EVENT_HPP
#define BLOCKCYPHER_EVENT_HPP

#include <QObject>
#include <QString>
#include <QJsonObject>

namespace BlockCypher {

#define MAX_CONFIRMATIONS_ALLOWED 10
#define MIN_CONFIDENCE 0
#define MAX_CONFIDENCE 1

    struct Event  {

        public:

            // Valid event types.
            enum class Type {
                unconfirmed_tx,
                new_block,
                confirmed_tx,
                tx_confirmation,
                double_spend_tx,
                tx_confidence
            };

            // Determine event Type from payload
            static Type getPayloadType(const QJsonObject & o);

            // Convernt between enum Type and string representation
            static Event::Type stringToType(const QString &s);
            static QString typeToString(Event::Type type);

            Event();

            Event(const QString & , const QString & token="");

            //Constructor aimed at tx-confidence event
            //event, confidence level, address, optional token
            Event(const QString & ,const float &, const QString &, const QString & token="");

            //Constructor aimed at tx-confirmation event
            //event,address,confirmations,optional token
            Event(const QString & ,const QString &,const unsigned short int &, const QString & token="");

            QJsonObject toJson() const;

            Event::Type type() const { return _event; }

            private:

            //Notes: Pr. 9th January 2016, BlockCypher does not support filtering pr. wallet. on their websocket.
            //This is on their "to-do" list and will be fixed. Remember to use ?token=YOURTOKEN once enabled.

            //hash, wallet_name and script are intentionally left out at this time.

            //See: http://dev.blockcypher.com/#types-of-events and http://dev.blockcypher.com/#event

            Type _event;            //Type of event; can be unconfirmed-tx, new-block, confirmed-tx, tx-confirmation, double-spend-tx, tx-confidence.

            //TODO: Future expansion, type safety for address?
            QString _address;       // Only transactions associated with the given address will be sent.
                                    // A wallet name can also be used instead of an address, which will
                                    // then match on any address in the wallet.
            unsigned short
            int _confirmations;     // Used in concert with the tx-confirmation event to determine how many notifications
                                    // are received. You’ll receive a notification with a TX payload for every confirmation
                                    // up to this number. The maximum allowed is 10; if not set, blockcypher websocket api
                                    // endpoint will set default to 6.

            float _confidence;      // Applies to tx-confidence event. Triggered any time an address has an unconfirmed
                                    // transaction above the confidence property specified in the Event, based on BlockCypher
                                    // Confidence Factor. The payload is the TX that triggered the event.
                                    // If confidence is not set, defaults to 0.99. Valid range [0-1] To ensure transactions
                                    // are not missed, even if your confidence threshold is not reached,
                                    // a transaction is still sent after a minute timeout; please remember
                                    // to double-check the confidence attribute in the TX payload.

            //What about typesafety here, QString ok?
            QString _token;         // Required if wallet_name is used, though generally we advise users
                                    // to include it (as they can reach API throttling thresholds rapidly).
    };

}

#endif // BLOCKCYPHER_EVENT_HPP
