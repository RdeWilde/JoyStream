/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 29 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_CURRENCYFIELDPREFIXSETTINGS_HPP
#define JOYSTREAM_CLASSIC_GUI_CURRENCYFIELDPREFIXSETTINGS_HPP

#include <common/BitcoinRepresentation.hpp>

class BitcoinDisplaySettings;

namespace joystream {
namespace classic {
namespace gui {

class CurrencyFieldPrefixSettings {

public:

    CurrencyFieldPrefixSettings(const BitcoinRepresentation::BitCoinPrefix & bitcoinPrefix,
                                const BitcoinRepresentation::MetricPrefix & metricPrefix,
                                const BitcoinDisplaySettings * settings);

    /**
     * @brief String encoded number of satoshies
     * @param numberOfSatoshies number of satoshies
     * @return
     */
    QString toString(quint64 numberOfSatoshies) const noexcept;

    /**
     * @brief toSatoshies
     * @param units
     * @return
     */
    quint64 toSatoshies(double units) const noexcept;

    QString prefix() const noexcept;

private:

    BitcoinRepresentation::BitCoinPrefix _bitcoinPrefix;
    BitcoinRepresentation::MetricPrefix _metricPrefix;

    const BitcoinDisplaySettings * _settings;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_CURRENCYFIELDPREFIXSETTINGS_HPP
