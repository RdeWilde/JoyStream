/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <cmath>
#include <cassert>

std::unordered_map<BitcoinRepresentation::BitCoinPrefix, int>
BitcoinRepresentation::bitCoinPrefixToPower = std::unordered_map<BitcoinRepresentation::BitCoinPrefix, int>
                                            {
                                                {BitcoinRepresentation::BitCoinPrefix::Satoshi, 0},
                                                {BitcoinRepresentation::BitCoinPrefix::Micro, 2},
                                                {BitcoinRepresentation::BitCoinPrefix::Milli, 5},
                                                {BitcoinRepresentation::BitCoinPrefix::Centi, 6},
                                                {BitcoinRepresentation::BitCoinPrefix::None, 8},
                                                {BitcoinRepresentation::BitCoinPrefix::Kilo, 11}
                                            };

std::unordered_map<int, BitcoinRepresentation::BitCoinPrefix>
BitcoinRepresentation::powerToBitCoinPrefix = std::unordered_map<int, BitcoinRepresentation::BitCoinPrefix>
                                            {
                                                {0, BitcoinRepresentation::BitCoinPrefix::Satoshi},
                                                {2, BitcoinRepresentation::BitCoinPrefix::Micro},
                                                {5, BitcoinRepresentation::BitCoinPrefix::Milli},
                                                {6, BitcoinRepresentation::BitCoinPrefix::Centi},
                                                {8, BitcoinRepresentation::BitCoinPrefix::None},
                                                {11, BitcoinRepresentation::BitCoinPrefix::Kilo}
                                            };

std::unordered_map<BitcoinRepresentation::MetricPrefix, int>
BitcoinRepresentation::metricPrefixToPower = std::unordered_map<BitcoinRepresentation::MetricPrefix, int>
                                            {
                                                {BitcoinRepresentation::MetricPrefix::Pico, -12},
                                                {BitcoinRepresentation::MetricPrefix::Nano, -9},
                                                {BitcoinRepresentation::MetricPrefix::Micro, -6},
                                                {BitcoinRepresentation::MetricPrefix::Milli, -3},
                                                {BitcoinRepresentation::MetricPrefix::Centi, -2},
                                                {BitcoinRepresentation::MetricPrefix::None, 0}
                                            };

std::unordered_map<int, BitcoinRepresentation::MetricPrefix>
BitcoinRepresentation::powerToMetricPrefix = std::unordered_map<int, BitcoinRepresentation::MetricPrefix>
                                            {
                                                {-12, BitcoinRepresentation::MetricPrefix::Pico},
                                                {-9, BitcoinRepresentation::MetricPrefix::Nano},
                                                {-6, BitcoinRepresentation::MetricPrefix::Micro},
                                                {-3, BitcoinRepresentation::MetricPrefix::Milli},
                                                {-2, BitcoinRepresentation::MetricPrefix::Centi},
                                                {0, BitcoinRepresentation::MetricPrefix::None}
                                            };

BitcoinRepresentation::BitcoinRepresentation(qint64 satoshies)
    : _satoshies(satoshies < 0 ? -satoshies : satoshies)
    , _isNegative(satoshies < 0 ? true : false) {

    Q_ASSERT(BitcoinRepresentation::bitCoinPrefixToPower.size() == BitcoinRepresentation::powerToBitCoinPrefix.size());
    Q_ASSERT(BitcoinRepresentation::metricPrefixToPower.size() == BitcoinRepresentation::powerToMetricPrefix.size());
}

BitcoinRepresentation::BitcoinRepresentation(BitCoinPrefix prefix, double quantity) {

    Q_ASSERT(BitcoinRepresentation::bitCoinPrefixToPower.size() == BitcoinRepresentation::powerToBitCoinPrefix.size());
    Q_ASSERT(BitcoinRepresentation::metricPrefixToPower.size() == BitcoinRepresentation::powerToMetricPrefix.size());

    if(quantity < 0) {
        _isNegative = true;
        quantity = -quantity;
    }

    // Compute amount of satoshies
    double numberOfSatoshies = pow(10, bitCoinPrefixToPower[prefix]) * quantity;

    /**
    // Check if we get integer number of satoshies, otherwise
    // quanitity has too many decimals for prefix.
    // NB!: Is this safe? what about floating point magic
    if(numberOfSatoshies != floor(numberOfSatoshies))
        throw std::exception("Invalid representation provided, does not give integer quanitity of satoshies.");
    else
        _satoshies = static_cast<quint64>(numberOfSatoshies);
    */

    // THERE IS NO GOOD WAY TO DEAL WITH BROKEN FLOATS COMING FROM BTC EXCHANGE RATES
    // SO WE JUST ROUND
    _satoshies = static_cast<quint64>(numberOfSatoshies);
}

BitcoinRepresentation::BitcoinRepresentation(MetricPrefix prefix, double fiatUnits, double fiatToBTCExchangeRate)
    : BitcoinRepresentation(BitCoinPrefix::None, (pow(10, metricPrefixToPower[prefix]) * fiatUnits)/fiatToBTCExchangeRate){
}

BitcoinRepresentation::BitCoinPrefix BitcoinRepresentation::bestPrefix() const {

    // Get the keys
    std::vector<int> keys;
    auto it = powerToBitCoinPrefix.begin();
    while(it != powerToBitCoinPrefix.cend()) {
        keys.push_back(it->first);
        ++it;
    }

    // Get best exponent
    int best = bestExponent(_satoshies, 10, keys);

    assert(powerToBitCoinPrefix.find(best) != powerToBitCoinPrefix.cend());

    // Return prefix
    return powerToBitCoinPrefix[best];
}

BitcoinRepresentation::MetricPrefix BitcoinRepresentation::bestPrefix(double fiatToBTCExchangeRate) const {

    double numberOfDollars = numberOfBTC() * fiatToBTCExchangeRate;

    // Get the keys
    std::vector<int> keys;
    auto it = powerToMetricPrefix.begin();
    while(it != powerToMetricPrefix.cend()) {
        keys.push_back(it->first);
        ++it;
    }

    // Get best exponent
    int best = bestExponent(numberOfDollars, 10, keys);

    assert(powerToMetricPrefix.find(best) != powerToMetricPrefix.cend());

    // Return prefix
    return powerToMetricPrefix[best];
}

int BitcoinRepresentation::bestExponent(double raw, quint8 base, const std::vector<int> & exponents) {

    // Find first available
    // power of base which is greater than raw

    // Counter
    int i = 0;

    // Power accumulator
    double power = pow(base, exponents[0]);

    while(raw > power && i < exponents.size()-1) {

        // Invariant: power = base^i && i < exponents.size()

        // Next power
        i++;

        // Compound power (here we rely on ascending size of exponents)
        power *= pow(base, exponents[i]-exponents[i-1]);
    }

    // If we stopped due to exhausting exponents, then use the last one
    if(raw > power)
        return exponents[i];
    // if we stopped due to firt exponent being to large, then use it
    else if(i == 0)
        return exponents[i];
    else {

        // base^(i-1) < raw < base^i, pick closest one

        double distanceToIthPower = power - raw;
        double distanceToIthMinusOnePower = raw - power/pow(base, exponents[i]-exponents[i-1]);

        if(distanceToIthPower < distanceToIthMinusOnePower)
            return exponents[i];
        else
            return exponents[i-1];
    }
}

double BitcoinRepresentation::unitsWithPrefix(BitCoinPrefix prefix) const {
    return ((double)_satoshies) / pow(10, bitCoinPrefixToPower[prefix]);
}

double BitcoinRepresentation::unitsWithPrefix(MetricPrefix prefix, double fiatToBTCExchangeRate) const {

    double totalAmountOfFiat = numberOfBTC() * fiatToBTCExchangeRate;

    return totalAmountOfFiat / pow(10, metricPrefixToPower[prefix]);
}

std::string BitcoinRepresentation::toString(BitCoinPrefix prefix, int precision) const {

    if(_satoshies == 0)
        return std::string("0B");
    else
        return (_isNegative ? std::string("-") : std::string("")) +
            std::to_string(roundf(unitsWithPrefix(prefix) * pow(10, precision))/pow(10, precision)) +
            " " + prefixToString(prefix) + std::string("B");
}

std::string BitcoinRepresentation::toString(int precision) const {
    return toString(bestPrefix(), precision);
}

std::string BitcoinRepresentation::toString(Fiat fiat, MetricPrefix prefix, double fiatToBTCExchangeRate, int precision) const {

    if(_satoshies == 0)
        return std::string("$0");
    else
        return (_isNegative ? std::string("-") : std::string("")) + fiatToSymbol(fiat) +
            std::to_string(roundf(unitsWithPrefix(prefix, fiatToBTCExchangeRate) * pow(10, precision))/pow(10, precision)) +
            " " + prefixToString(prefix);
}

std::string BitcoinRepresentation::toString(Fiat fiat, double fiatToBTCExchangeRate, int precision) const {
    return toString(fiat, bestPrefix(fiatToBTCExchangeRate), fiatToBTCExchangeRate, precision);
}

std::string BitcoinRepresentation::toString(const BitcoinDisplaySettings * settings) const {

    switch(settings->currency()) {

        case BitcoinDisplaySettings::Currency::BitCoin:
            return toString(settings->precision());
        case BitcoinDisplaySettings::Currency::Fiat:
            return toString(settings->fiat(), settings->rate(), settings->precision());
        default:
            Q_ASSERT(false);
    }
}

double BitcoinRepresentation::numberOfBTC() const {

    quint64 satoshiesInABitCoin = pow(10, bitCoinPrefixToPower[BitCoinPrefix::None]);
    double numberOfBitCoins = ((double)_satoshies) / satoshiesInABitCoin;

    return numberOfBitCoins;
}

quint64 BitcoinRepresentation::satoshies() const {
    return _satoshies;
}

void BitcoinRepresentation::setSatoshies(quint64 satoshies) {
    _satoshies = satoshies;
}

std::string BitcoinRepresentation::prefixToString(BitCoinPrefix prefix) {

    switch(prefix) {

        case BitCoinPrefix::Satoshi: return "s";
        case BitCoinPrefix::Micro: return "μ";
        case BitCoinPrefix::Milli: return "m";
        case BitCoinPrefix::Centi: return "c";
        case BitCoinPrefix::None: return "";
        case BitCoinPrefix::Kilo: return "k";

        default:
            Q_ASSERT(false);
    }
}

std::string BitcoinRepresentation::prefixToString(MetricPrefix prefix) {

    switch(prefix) {
        case MetricPrefix::Pico: return std::string("p");
        case MetricPrefix::Nano: return std::string("n");
        case MetricPrefix::Micro: return std::string("μ");
        case MetricPrefix::Milli: return std::string("m");
        case MetricPrefix::Centi: return std::string("¢");
        case MetricPrefix::None: return std::string("");

        default:
            Q_ASSERT(false);
    }
}

/**
quint8 BitCoinRepresentation::prefixToExponent(BitCoinPrefix prefix) {

    switch(prefix) {
        case BitCoinPrefix::Satoshi: return 0;
        case BitCoinPrefix::Micro: return 2;
        case BitCoinPrefix::Milli: return 5;
        case BitCoinPrefix::Centi: return 6;
        case BitCoinPrefix::None: return 8;
        case BitCoinPrefix::Kilo: return 11;

        default:
            Q_ASSERT(false);
    }
}


int BitCoinRepresentation::prefixToExponent(MetricPrefix prefix) {

    switch(prefix) {
        case MetricPrefix::Pico: return -12;
        case MetricPrefix::Nano: return -9;
        case MetricPrefix::Micro: return -6;
        case MetricPrefix::Milli: return -3;
        case MetricPrefix::Centi: return -2;
        case MetricPrefix::None: return 0;

        default:
            Q_ASSERT(false);
    }
}
*/

std::string BitcoinRepresentation::fiatToSymbol(Fiat fiat) {

    switch(fiat) {
        case Fiat::USD: return std::string("$");
        case Fiat::Euro: return std::string("€");
        case Fiat::Pound: return std::string("£");
        case Fiat::Yen: return std::string("¥");

        default:
            Q_ASSERT(false);
    }
}
