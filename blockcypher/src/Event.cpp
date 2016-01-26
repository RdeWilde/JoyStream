

#include <blockcypher/Event.hpp>

#include <QDebug>

namespace BlockCypher {

    // ===== C O N S T R U C T O R S

    Event::Event() {}

    //Note, token is optional.
    Event::Event(const QString & event, const QString &token) : _event(toType(event)), _token(token) { }

    //tx-confidence event. Note, token is optional.
    Event::Event(const QString & event,const float &c, const QString &adr, const QString &token ) :
        _event(toType(event)),_address(adr),_confidence(c),_token(token) { }

    //tx-confirmation event. Note, token is optional.
    Event::Event(const QString & event,const QString &adr,const unsigned short int &conf, const QString &token) :
        _event(toType(event)),_address(adr),_confirmations(conf),_token(token) { }


    // ===== S E T  M E T H O D S

    void Event::setEvent(QString t) {
        _event = toType(t);
    }

    void Event::setAddress(const QString &adr) {
        //TODO: Check if address is valid based on blockchain used?
        _address = adr;
    }

    const bool Event::setConfirmations(const unsigned short int &conf) {
        if (conf > MAX_CONFIRMATIONS_ALLOWED) {
            return false;
        }
        _confirmations = conf;
        return true;
    }

    const bool Event::setConfidence(const float &c) {
        if (c < MIN_CONFIDENCE || c >MAX_CONFIDENCE) {
            return false;
        }

        _confidence = c;
        return true;
    }

    void Event::setToken(const QString & token) {
        _token = token;
    }


    // ===== G E T  M E T H O D S

    Event::Type Event::getEvent() const{
        return _event;
    }

    QString Event::getAddress() const {
        return _address;
    }

    unsigned short int Event::getConfirmations() const{
        return _confirmations;
    }

    float Event::getConfidence() const{
        return _confidence;
    }

    QString Event::getToken() const {
        return _token;
    }


    // ===== E N U M  M E T H O D S

    Event::Type Event::toType(const QString &s) const {

        if(s == "unconfirmed-tx")        return Event::Type::unconfirmed_tx;
        else if(s == "new-block")        return Event::Type::new_block;
        else if(s == "confirmed-tx")     return Event::Type::confirmed_tx;
        else if(s == "tx-confirmation")  return Event::Type::tx_confirmation;
        else if(s == "double-spend-tx")  return Event::Type::double_spend_tx;
        else if(s == "tx-confidence")    return Event::Type::tx_confidence;
        else Q_ASSERT(false);

    }

    QString   Event::fromType(Event::Type type) const {

        switch(type) {
            case Event::Type::unconfirmed_tx: return "unconfirmed-tx";
            case Event::Type::new_block: return "new-block";
            case Event::Type::confirmed_tx: return "confirmed-tx";
            case Event::Type::tx_confirmation: return "tx-confirmation";
            case Event::Type::double_spend_tx: return "double-spend-tx";
            case Event::Type::tx_confidence: return "tx-confidence";
            default: Q_ASSERT(false);
        }
    }

    Event::Type Event::getPayloadType(const QJsonObject & o) {
        throw std::runtime_error("not implemented");
    }


    // ===== J S O N  M E T H O D S


    //Converts Event object to JSON.
    QJsonObject Event::toJson() const{

        QJsonObject json_data;

        json_data["event"] = Event::fromType(_event);


        if(!_token.isEmpty())
            json_data["token"] = _token;

        //We only need event member
        if(_event == Event::Type::unconfirmed_tx ||
                _event == Event::Type::new_block ||
                _event == Event::Type::confirmed_tx ||
                _event == Event::Type::double_spend_tx) {
            return json_data;
        }

        //Remaining possible events tx-confirmation and tx-confidence.

        if(_event == Event::Type::tx_confirmation) {
            json_data["confirmations"] = _confirmations;
        }

        if(_event == Event::Type::tx_confidence) {
            json_data["confidence"] = _confidence;
        }

        json_data["address"] = _address;

        return json_data;
    }

} // end namespace BlockCypher




