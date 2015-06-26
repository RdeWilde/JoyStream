/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "BitCoinDisplaySettings.hpp"

BitCoinDisplaySettings::BitCoinDisplaySettings(int precision)
    : _currency(Currency::BitCoin)
    , _precision(precision) {
}

BitCoinDisplaySettings::BitCoinDisplaySettings(Fiat fiat, float rate, int precision)
    : _currency(Currency::Fiat)
    , _fiat(fiat)
    , _rate(rate)
    , _precision(precision) {
}

BitCoinDisplaySettings::Currency BitCoinDisplaySettings::currency() const {
    return _currency;
}

void BitCoinDisplaySettings::setCurrency(Currency currency) {
    _currency = currency;
}

Fiat BitCoinDisplaySettings::fiat() const {
    return _fiat;
}

void BitCoinDisplaySettings::setFiat(Fiat fiat) {
    _fiat = fiat;
}

float BitCoinDisplaySettings::rate() const {
    return _rate;
}

void BitCoinDisplaySettings::setRate(float rate) {
    _rate = rate;
}

int BitCoinDisplaySettings::precision() const {
    return _precision;
}

void BitCoinDisplaySettings::setPrecision(int value) {
    _precision = value;
}

