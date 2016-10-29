#ifndef JOYSTREAM_APPKIT_TORRENTPLUGINSTATE_HPP
#define JOYSTREAM_APPKIT_TORRENTPLUGINSTATE_HPP

#include <core/TorrentPlugin.hpp>

#include <QJsonObject>

namespace joystream {
namespace appkit {

class TorrentPluginState
{
public:
    TorrentPluginState();

    TorrentPluginState(const core::TorrentPlugin*);

    TorrentPluginState(const QJsonValue&);

    QJsonValue toJson() const;

    protocol_session::SessionMode mode() const;
    protocol_session::SessionState state() const;

    // Use the internal state information and attempt to transition a plugin to that state
    void applyTo(core::TorrentPlugin* /*, callback */) const;

    protocol_wire::BuyerTerms buyerTerms() const;
    protocol_wire::SellerTerms sellerTerms() const;
    protocol_session::BuyingPolicy buyingPolicy() const;
    protocol_session::SellingPolicy sellingPolicy() const;

private:

    protocol_session::SessionMode _mode;

    protocol_session::SessionState _state;

    protocol_wire::BuyerTerms _buyerTerms;
    protocol_session::BuyingPolicy _buyingPolicy;

    protocol_session::SellingPolicy _sellingPolicy;
    protocol_wire::SellerTerms _sellerTerms;
};

}
}

#endif // JOYSTREAM_APPKIT_TORRENTPLUGINSTATE_HPP
