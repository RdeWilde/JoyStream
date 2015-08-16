/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_DATA_SIZE_REPRESENTATION_HPP
#define COIN_DATA_SIZE_REPRESENTATION_HPP

#include <QtGlobal>

class QString;

// Manages data size representations
// Based on http://en.wikipedia.org/wiki/JEDEC_memory_standards#Unit_prefixes_for_semiconductor_storage_capacity
// extended with extra prefixes only present in the metric case.
class DataSizeRepresentation
{
public:

    // Prefix used for representation
    // ======================
    // It determines the exponent on the base size
    enum class Prefix {
        None,  //^0
        Kilo,  //^1
        Mega,  //^2
        Giga,  //^3
        Tera,  //^4
        Peta,  //^5
        Exa,   //^6
        Zetta, //^7
        Yotta  //^8
    };

    static const quint8 maxPower; // = 8

    // Base of representation
    // ======================
    // Bit is typically used for transmitted data,
    // while Byte for stored data, and this modulates
    // the base of the prefix power
    // Bit => 1024^prefix
    // Byte => 1000^prefix

    enum class Base {
        Bit,
        Byte
    };

    // How prefix and base should be represented as text
    // ======================
    // Prefix: K vs kilo, M vs mega, ...
    // Base: B vs byte, and bit is always just bit
    enum class TextFormat {
        Short,
        Long
    };

    // Constructor
    DataSizeRepresentation(quint64 numberOfBaseUnits, Base base);

    // Copy constructor
    DataSizeRepresentation(const DataSizeRepresentation & o);

    // Returns the prefix
    Prefix bestPrefix() const;

    // Computes the significand for given prefix
    double unitsWithPrefix(Prefix prefix) const;

    // Returns string form of representation
    QString toString(Prefix prefix, TextFormat format = TextFormat::Short, int precision = 1) const;
    QString toString(TextFormat format = TextFormat::Short, int precision = 1) const;

    // Getters and setters
    quint64 numberOfBaseUnits() const;
    void setNumberOfBaseUnits(quint64 numberOfBaseUnits);

    Base base() const;
    void setBase(Base base);

private:

    // The number of base units of data
    quint64 _numberOfBaseUnits;

    // Base for representation
    Base _base;

    // Static utilities
    static QString prefixToString(Prefix prefix, TextFormat format = TextFormat::Short);
    static QString baseToString(Base base, TextFormat format = TextFormat::Short);

    static quint8 prefixToExponent(Prefix prefix);
    static Prefix exponentToPrefix(quint8 power);
    static quint16 sizeOfBase(Base base);
};

#endif // COIN_DATA_SIZE_REPRESENTATION_HPP
