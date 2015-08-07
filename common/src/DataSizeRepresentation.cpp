/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/DataSizeRepresentation.hpp>

#include <QString>

#include <cmath>

const quint8 DataSizeRepresentation::maxPower = 8;

DataSizeRepresentation::DataSizeRepresentation(quint64 numberOfBaseUnits, Base base)
    : _numberOfBaseUnits(numberOfBaseUnits)
    , _base(base) {
}

DataSizeRepresentation::DataSizeRepresentation(const DataSizeRepresentation & o)
    : DataSizeRepresentation(o.numberOfBaseUnits(), o.base()) {
}

DataSizeRepresentation::Prefix DataSizeRepresentation::bestPrefix() const {

    quint16 size = sizeOfBase(_base);

    // Find greatest power of base size which is greater than _numberOfBaseUnits

    // Keeps track of power found so far
    quint64 accumulator = 1;

    // Exponent of base size used in accumulator
    int exponent = 0;

    while(exponent <= DataSizeRepresentation::maxPower &&
          accumulator <= _numberOfBaseUnits) {

        accumulator *= size;
        exponent++;
    }

    // Taking no exponent was big enough
    if(exponent == 0) {
        Q_ASSERT(_numberOfBaseUnits == 0);
        return Prefix::None;
    }

    // If no exponent is big enough, then just use biggest power Yotta
    if(accumulator <= _numberOfBaseUnits)
        return Prefix::Yotta;

    // Return the prefix corresponding to the power, among i and i-1 of base size,
    // to which _numberOfBaseUnits is closest
    quint64 distanceToIthPower = accumulator - _numberOfBaseUnits;
    quint64 distanceToIthMinusOnePower = -(accumulator/size - _numberOfBaseUnits);

    if(distanceToIthPower <= distanceToIthMinusOnePower)
        return exponentToPrefix(exponent);
    else
        return exponentToPrefix(exponent-1);
}

double DataSizeRepresentation::unitsWithPrefix(Prefix prefix) const {
    return ((double)_numberOfBaseUnits) / pow(sizeOfBase(_base), prefixToExponent(prefix));
}

QString DataSizeRepresentation::toString(Prefix prefix, TextFormat format, int precision) const {
    return QString::number(unitsWithPrefix(prefix), 'f', precision) + " " + prefixToString(prefix, format) + baseToString(_base, format);
}

QString DataSizeRepresentation::toString(TextFormat format, int precision) const {
    return toString(bestPrefix(), format, precision);
}

quint64 DataSizeRepresentation::numberOfBaseUnits() const {
    return _numberOfBaseUnits;
}

void DataSizeRepresentation::setNumberOfBaseUnits(quint64 quantity) {
    _numberOfBaseUnits = quantity;
}

DataSizeRepresentation::Base DataSizeRepresentation::base() const {
    return _base;
}

void DataSizeRepresentation::setBase(Base base) {
    _base = base;
}


QString DataSizeRepresentation::prefixToString(Prefix prefix, TextFormat format) {

    switch(prefix) {
        case Prefix::None: return "";
        case Prefix::Kilo: return (format == TextFormat::Short ? "K" : "kilo");
        case Prefix::Mega: return (format == TextFormat::Short ? "M" : "mega");
        case Prefix::Giga: return (format == TextFormat::Short ? "G" : "giga");
        case Prefix::Tera: return (format == TextFormat::Short ? "T" : "terra");
        case Prefix::Peta: return (format == TextFormat::Short ? "P" : "peta");
        case Prefix::Exa: return (format == TextFormat::Short ? "E" : "exa");
        case Prefix::Zetta: return (format == TextFormat::Short ? "Z" : "zetta");
        case Prefix::Yotta: return (format == TextFormat::Short ? "Y" : "yotta");
        default:
            Q_ASSERT(false);
    }
}

QString DataSizeRepresentation::baseToString(Base base, TextFormat format) {

    switch(base) {
        case Base::Bit: return "bit"; // is insensitive to format
        case Base::Byte: return (format == TextFormat::Short ? "B" : "byte");
        default:
            Q_ASSERT(false);
    }
}

quint8 DataSizeRepresentation::prefixToExponent(Prefix prefix) {

    switch(prefix) {
        case Prefix::None: return 0;
        case Prefix::Kilo: return 1;
        case Prefix::Mega: return 2;
        case Prefix::Giga: return 3;
        case Prefix::Tera: return 4;
        case Prefix::Peta: return 5;
        case Prefix::Exa: return 6;
        case Prefix::Zetta: return 7;
        case Prefix::Yotta: return 8;
        default:
            Q_ASSERT(false);
    }
}


DataSizeRepresentation::Prefix DataSizeRepresentation::exponentToPrefix(quint8 exponent) {

    switch(exponent) {
        case 0: return Prefix::None;
        case 1: return Prefix::Kilo;
        case 2: return Prefix::Mega;
        case 3: return Prefix::Giga;
        case 4: return Prefix::Tera;
        case 5: return Prefix::Peta;
        case 6: return Prefix::Exa;
        case 7: return Prefix::Zetta;
        case 8: return Prefix::Yotta;
        default:
            Q_ASSERT(false);
    }
}

quint16 DataSizeRepresentation::sizeOfBase(Base base) {

    switch(base) {
        case Base::Bit: return 1024;
        case Base::Byte: return 1000;
        default:
            Q_ASSERT(false);
    }
}
