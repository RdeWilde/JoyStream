/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

#ifndef JOYSTREAM_APPKIT_TORRENTPLUGINSTATE_HPP
#define JOYSTREAM_APPKIT_TORRENTPLUGINSTATE_HPP

#include <core/TorrentPlugin.hpp>

#include <QJsonObject>

namespace joystream {
namespace appkit {

class SavedSessionParameters
{
public:
    SavedSessionParameters();

    SavedSessionParameters(const core::TorrentPlugin*);

    SavedSessionParameters(const QJsonValue&);

    QJsonValue toJson() const;

    protocol_session::SessionMode mode() const;
    protocol_session::SessionState state() const;

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
