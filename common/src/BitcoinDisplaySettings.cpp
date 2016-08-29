/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/BitcoinDisplaySettings.hpp>

BitcoinDisplaySettings::BitcoinDisplaySettings(int precision)
    : _currency(Currency::BitCoin)
    , _precision(precision) {
}

BitcoinDisplaySettings::BitcoinDisplaySettings(Fiat fiat, float rate, int precision)
    : _currency(Currency::Fiat)
    , _fiat(fiat)
    , _rate(rate)
    , _precision(precision) {
}

BitcoinDisplaySettings::Currency BitcoinDisplaySettings::currency() const {
    return _currency;
}

void BitcoinDisplaySettings::setCurrency(Currency currency) {

    if(_currency != currency)
        emit changed();

    _currency = currency;
}

Fiat BitcoinDisplaySettings::fiat() const {
    return _fiat;
}

void BitcoinDisplaySettings::setFiat(Fiat fiat) {

    if(_fiat != fiat)
        emit changed();

    _fiat = fiat;
}

float BitcoinDisplaySettings::rate() const {
    return _rate;
}

void BitcoinDisplaySettings::setRate(float rate) {

    if(_rate != rate)
        emit changed();

    _rate = rate;
}

int BitcoinDisplaySettings::precision() const {
    return _precision;
}

void BitcoinDisplaySettings::setPrecision(int precision) {

    if(_precision != precision)
        emit changed();

    _precision = precision;
}

