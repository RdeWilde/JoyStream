/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 29 2016
 */

#include <gui/CurrencyFieldPrefixSettings.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <cassert>

namespace joystream {
namespace classic {
namespace gui {

CurrencyFieldPrefixSettings::CurrencyFieldPrefixSettings(const BitcoinRepresentation::BitCoinPrefix & bitcoinPrefix,
                                                         const BitcoinRepresentation::MetricPrefix & metricPrefix,
                                                         const BitcoinDisplaySettings * settings)
    : _bitcoinPrefix(bitcoinPrefix)
    , _metricPrefix(metricPrefix)
    , _settings(settings) {
}

QString CurrencyFieldPrefixSettings::toString(quint64 numberOfSatoshies) {

    BitcoinRepresentation representation(numberOfSatoshies);

    switch(_settings->currency()) {

        case BitcoinDisplaySettings::Currency::BitCoin:

            return representation.toString(_bitcoinPrefix);

        case BitcoinDisplaySettings::Currency::Fiat:

            return representation.toString(_settings->fiat(), _metricPrefix, _settings->rate());

        default:
            assert(false);
    }

}

quint64 CurrencyFieldPrefixSettings::toSatoshies(double units) {

    switch(_settings->currency()) {

        case BitcoinDisplaySettings::Currency::BitCoin:

            return BitcoinRepresentation(_bitcoinPrefix, units).satoshies();

        case BitcoinDisplaySettings::Currency::Fiat:

            return BitcoinRepresentation(_metricPrefix, units, _settings->rate()).satoshies();

        default:
            assert(false);
    }

}

}
}
}
