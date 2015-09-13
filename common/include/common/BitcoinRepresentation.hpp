/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_BITCOIN_REPRESENTATION_HPP
#define COIN_BITCOIN_REPRESENTATION_HPP

#include "Fiat.hpp"

#include <QtGlobal>
#include <QMap>

class QString;
class BitcoinDisplaySettings;

// Manage crypto currency representaion
// Should probably just have static routines
class BitcoinRepresentation
{
public:

    // Prefix used for bitcoin
    // Based on https://en.bitcoin.it/wiki/Units
    // ======================
    enum class BitCoinPrefix {
        Satoshi,    // 10^0
        //Finney,   // 10^1
        Micro,      // 10^2, also called: bit
        Milli,      // 10^5, also called: millibit, millicoin, millie
        Centi,      // 10^6, also called: bitcent
        //Deci,     // 10^7
        None,       // 10^8, 1 BTC
        //Deca,     // 10^9
        Kilo        // 10^(8+3)
        //Mega      // 10^(8+3+3)
    };

    // Bitcoin prefix to power bijection
    static QMap<BitCoinPrefix, int> bitCoinPrefixToPower;
    static QMap<int, BitCoinPrefix> powerToBitCoinPrefix;

    // Prefix used for fiat currency
    // Based on http://en.wikipedia.org/wiki/Metric_prefix
    enum class MetricPrefix {
        Pico,   // 10^-12
        Nano,   // 10^-9
        Micro,  // 10^-6
        Milli,  // 10^-3
        Centi,  // 10^-2
        None    // 10^0 (unity)
    };

    // Metric prefix to power bijection
    static QMap<MetricPrefix, int> metricPrefixToPower;
    static QMap<int, MetricPrefix> powerToMetricPrefix;

    /**
     * Constructors
     */

    // Constructor from raw number of satoshies
    BitcoinRepresentation(qint64 satoshies);

    // Constructor from prefixed crypto currency amount
    BitcoinRepresentation(BitCoinPrefix prefix, double units);

    // Constructor from fiat
    // fiatToBTCExchangeRate := number of fiat units BitCoinPrefix::None units of satoshies buys
    BitcoinRepresentation(MetricPrefix prefix, double fiatUnits, double fiatToBTCExchangeRate);

    /**
     * Convert to QString routines
     */

    // BitCoin representation with best prefix
    QString toString(int precision = 1) const;

    // Fiat representation with best prefix
    QString toString(Fiat fiat, double fiatToBTCExchangeRate, int precision = 1) const;

    // Representation based on display settings
    QString toString(const BitcoinDisplaySettings * settings) const;

    // Number of BitCoins
    // Is very useful since dealing with fiat requires dealing with exchange rates,
    // which are always /btc.
    double numberOfBTC() const;

    quint64 satoshies() const;
    void setSatoshies(quint64 satoshies);

private:

    // Raw number of satoshies, is always positive
    quint64 _satoshies;

    // Whether amount is negative
    bool _isNegative;

    /**
     * Prefix processing
     */

    // Returns best prefix
    BitCoinPrefix bestPrefix() const;
    MetricPrefix bestPrefix(double fiatToBTCExchangeRate) const;

    // Computes the significand for given prefix
    double unitsWithPrefix(BitCoinPrefix prefix) const;
    double unitsWithPrefix(MetricPrefix prefix, double fiatToBTCExchangeRate) const;

    /**
     * To string conversion
     */

    // Find best exponent for
    // Move into some utilitie wrapper later, shouldnt be here, can be used
    // by data representation class also
    static int bestExponent(double raw, quint8 base, const QList<int> & exponents);

    // BitCoin representation with given prefix
    QString toString(BitCoinPrefix prefix, int precision = 1) const;

    // Fiat representation with given prefix
    QString toString(Fiat fiat, MetricPrefix prefix, double fiatToBTCExchangeRate, int precision = 1) const;

    // Static utilities
    static quint64 satoshiesInABTC();

    static QString prefixToString(BitCoinPrefix prefix);
    static QString prefixToString(MetricPrefix prefix);

    //static quint8 prefixToExponent(BitCoinPrefix prefix);
    //static int prefixToExponent(MetricPrefix prefix);

    static QString fiatToSymbol(Fiat fiat);
};

#endif // COIN_BITCOIN_REPRESENTATION_HPP
