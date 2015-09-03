/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 2 2015
 */

#ifndef SIGHASHTYPE_HPP
#define SIGHASHTYPE_HPP

namespace Coin {

/**
 * Bitcoin network
 */
enum class SigHashType {
    all,
    none,
    single,
    anyonecanpay
};

/**
 * Conversion routines
 */

unsigned char valueForSighashType(SigHashType type);
SigHashType SigHashTypeFromValue(unsigned char value);

}

#endif // SIGHASHTYPE_HPP
