#include "BitCoinRepresentation.hpp"
#include "BitCoinDisplaySettings.hpp"

#include <QString>

#include <cmath>

QMap<BitCoinRepresentation::BitCoinPrefix, int>
BitCoinRepresentation::bitCoinPrefixToPower = QMap<BitCoinRepresentation::BitCoinPrefix, int>
                                            {
                                                {BitCoinRepresentation::BitCoinPrefix::Satoshi, 0},
                                                {BitCoinRepresentation::BitCoinPrefix::Micro, 2},
                                                {BitCoinRepresentation::BitCoinPrefix::Milli, 5},
                                                {BitCoinRepresentation::BitCoinPrefix::Centi, 6},
                                                {BitCoinRepresentation::BitCoinPrefix::None, 8},
                                                {BitCoinRepresentation::BitCoinPrefix::Kilo, 11}
                                            };

QMap<int, BitCoinRepresentation::BitCoinPrefix>
BitCoinRepresentation::powerToBitCoinPrefix = QMap<int, BitCoinRepresentation::BitCoinPrefix>
                                            {
                                                {0, BitCoinRepresentation::BitCoinPrefix::Satoshi},
                                                {2, BitCoinRepresentation::BitCoinPrefix::Micro},
                                                {5, BitCoinRepresentation::BitCoinPrefix::Milli},
                                                {6, BitCoinRepresentation::BitCoinPrefix::Centi},
                                                {8, BitCoinRepresentation::BitCoinPrefix::None},
                                                {11, BitCoinRepresentation::BitCoinPrefix::Kilo}
                                            };

QMap<BitCoinRepresentation::MetricPrefix, int>
BitCoinRepresentation::metricPrefixToPower = QMap<BitCoinRepresentation::MetricPrefix, int>
                                            {
                                                {BitCoinRepresentation::MetricPrefix::Pico, -12},
                                                {BitCoinRepresentation::MetricPrefix::Nano, -9},
                                                {BitCoinRepresentation::MetricPrefix::Micro, -6},
                                                {BitCoinRepresentation::MetricPrefix::Milli, -3},
                                                {BitCoinRepresentation::MetricPrefix::Centi, -2},
                                                {BitCoinRepresentation::MetricPrefix::None, 0}
                                            };

QMap<int, BitCoinRepresentation::MetricPrefix>
BitCoinRepresentation::powerToMetricPrefix = QMap<int, BitCoinRepresentation::MetricPrefix>
                                            {
                                                {-12, BitCoinRepresentation::MetricPrefix::Pico},
                                                {-9, BitCoinRepresentation::MetricPrefix::Nano},
                                                {-6, BitCoinRepresentation::MetricPrefix::Micro},
                                                {-3, BitCoinRepresentation::MetricPrefix::Milli},
                                                {-2, BitCoinRepresentation::MetricPrefix::Centi},
                                                {0, BitCoinRepresentation::MetricPrefix::None}
                                            };

BitCoinRepresentation::BitCoinRepresentation(qint64 satoshies)
    : _satoshies(satoshies < 0 ? -satoshies : satoshies)
    , _isNegative(satoshies < 0 ? true : false) {

    Q_ASSERT(BitCoinRepresentation::bitCoinPrefixToPower.size() == BitCoinRepresentation::powerToBitCoinPrefix.size());
    Q_ASSERT(BitCoinRepresentation::metricPrefixToPower.size() == BitCoinRepresentation::powerToMetricPrefix.size());
}

BitCoinRepresentation::BitCoinRepresentation(BitCoinPrefix prefix, double quantity) {

    Q_ASSERT(BitCoinRepresentation::bitCoinPrefixToPower.size() == BitCoinRepresentation::powerToBitCoinPrefix.size());
    Q_ASSERT(BitCoinRepresentation::metricPrefixToPower.size() == BitCoinRepresentation::powerToMetricPrefix.size());

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

BitCoinRepresentation::BitCoinRepresentation(MetricPrefix prefix, double fiatUnits, double fiatToBTCExchangeRate)
    : BitCoinRepresentation(BitCoinPrefix::None, (pow(10, metricPrefixToPower[prefix]) * fiatUnits)/fiatToBTCExchangeRate){
}

BitCoinRepresentation::BitCoinPrefix BitCoinRepresentation::bestPrefix() const {

    // Get best exponent
    int best = bestExponent(_satoshies, 10, powerToBitCoinPrefix.keys());

    Q_ASSERT(powerToBitCoinPrefix.contains(best));

    // Return prefix
    return powerToBitCoinPrefix[best];
}

BitCoinRepresentation::MetricPrefix BitCoinRepresentation::bestPrefix(double fiatToBTCExchangeRate) const {

    double numberOfDollars = numberOfBTC() * fiatToBTCExchangeRate;

    // Get best exponent
    int best = bestExponent(numberOfDollars, 10, powerToMetricPrefix.keys());

    Q_ASSERT(powerToMetricPrefix.contains(best));

    // Return prefix
    return powerToMetricPrefix[best];
}

int BitCoinRepresentation::bestExponent(double raw, quint8 base, const QList<int> & exponents) {

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

double BitCoinRepresentation::unitsWithPrefix(BitCoinPrefix prefix) const {
    return ((double)_satoshies) / pow(10, bitCoinPrefixToPower[prefix]);
}

double BitCoinRepresentation::unitsWithPrefix(MetricPrefix prefix, double fiatToBTCExchangeRate) const {

    double totalAmountOfFiat = numberOfBTC() * fiatToBTCExchangeRate;

    return totalAmountOfFiat / pow(10, metricPrefixToPower[prefix]);
}

QString BitCoinRepresentation::toString(BitCoinPrefix prefix, int precision) const {

    if(_satoshies == 0)
        return QString("0Ƀ");
    else
        return (_isNegative ? QString("-") : QString("")) + QString::number(unitsWithPrefix(prefix), 'f', precision) + " " + prefixToString(prefix) + QString("Ƀ");
}

QString BitCoinRepresentation::toString(int precision) const {
    return toString(bestPrefix(), precision);
}

QString BitCoinRepresentation::toString(Fiat fiat, MetricPrefix prefix, double fiatToBTCExchangeRate, int precision) const {

    if(_satoshies == 0)
        return QString("$0");
    else
        return (_isNegative ? QString("-") : QString("")) + fiatToSymbol(fiat) + QString::number(unitsWithPrefix(prefix, fiatToBTCExchangeRate), 'f', precision) + " " + prefixToString(prefix);
}

QString BitCoinRepresentation::toString(Fiat fiat, double fiatToBTCExchangeRate, int precision) const {
    return toString(fiat, bestPrefix(fiatToBTCExchangeRate), fiatToBTCExchangeRate, precision);
}

QString BitCoinRepresentation::toString(const BitCoinDisplaySettings * settings) const {

    switch(settings->currency()) {

        case BitCoinDisplaySettings::Currency::BitCoin:
            return toString(settings->precision());
        case BitCoinDisplaySettings::Currency::Fiat:
            return toString(settings->fiat(), settings->rate(), settings->precision());
        default:
            Q_ASSERT(false);
    }
}

double BitCoinRepresentation::numberOfBTC() const {

    quint64 satoshiesInABitCoin = pow(10, bitCoinPrefixToPower[BitCoinPrefix::None]);
    double numberOfBitCoins = ((double)_satoshies) / satoshiesInABitCoin;

    return numberOfBitCoins;
}

quint64 BitCoinRepresentation::satoshies() const {
    return _satoshies;
}

void BitCoinRepresentation::setSatoshies(quint64 satoshies) {
    _satoshies = satoshies;
}

QString BitCoinRepresentation::prefixToString(BitCoinPrefix prefix) {

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

QString BitCoinRepresentation::prefixToString(MetricPrefix prefix) {

    switch(prefix) {
        case MetricPrefix::Pico: return QString("p");
        case MetricPrefix::Nano: return QString("n");
        case MetricPrefix::Micro: return QString("μ");
        case MetricPrefix::Milli: return QString("m");
        case MetricPrefix::Centi: return QString("¢");
        case MetricPrefix::None: return QString("");

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

QString BitCoinRepresentation::fiatToSymbol(Fiat fiat) {

    switch(fiat) {
        case Fiat::USD: return QString("$");
        case Fiat::Euro: return QString("€");
        case Fiat::Pound: return QString("£");
        case Fiat::Yen: return QString("¥");

        default:
            Q_ASSERT(false);
    }
}
